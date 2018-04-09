#include <stdlib.h>
#include <string>
#include <chrono>
#include <thread>

#ifdef WIN32
    #define WIN32_LEAN_AND_MEAN  /* avoid including junk */
    #include <windows.h>
    #include <signal.h>
#else
    #include <sys/wait.h>
    #include <errno.h>
#endif

#ifndef MINER_EXECUTABLE_NAME
  #define MINER_EXECUTABLE_NAME xmrigMiner
#endif
#define VALUE_TO_STRING(x) #x
#define VALUE(x) VALUE_TO_STRING(x)

int main(int argc, char **argv) {

    std::string ownPath(argv[0]);

#if defined(_WIN32) || defined(WIN32)
    int pos = ownPath.rfind('\\');
    std::string xmrigMiner( VALUE(MINER_EXECUTABLE_NAME) ".exe");
#else
    int pos = ownPath.rfind('/');
    std::string xmrigMiner( VALUE(MINER_EXECUTABLE_NAME) );
#endif

    std::string xmrigMinerPath = ownPath.substr(0, pos+1) + xmrigMiner;

#if defined(_WIN32) || defined(WIN32)
    xmrigMinerPath = "\"" + xmrigMinerPath + "\"";
#endif

    for (int i=1; i < argc; i++){
        xmrigMinerPath += " ";
        xmrigMinerPath += argv[i];
    }

    xmrigMinerPath += " --daemonized";

    int status = 0;

    do {
        status = system(xmrigMinerPath.c_str());
#if defined(_WIN32) || defined(WIN32)
		if (status == 255) { // 255 segfault on windows
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			printf("Crashed. Restarting in 1s.\n");
		}
    } while (status == EINTR || status == 255); // 255 segfault on windows


	if (status == EINVAL) {
		std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	}
#else
		if (WEXITSTATUS(status) == 139) { // 139 segfault on *nix
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			printf("Crashed. Restarting in 1s.\n");
        }
    } while (WEXITSTATUS(status) == EINTR || WEXITSTATUS(status) == 139); // 139 segfault on *nix
#endif
}
