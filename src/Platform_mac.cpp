#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <uv.h>


#include "Platform.h"
#include "version.h"

#ifdef XMRIG_NVIDIA_PROJECT
#   include "nvidia/cryptonight.h"
#endif


static inline char *createUserAgent()
{
    const size_t max = 160;

    char *buf = new char[max];

#   ifdef XMRIG_NVIDIA_PROJECT
    const int cudaVersion = cuda_get_runtime_version();
    snprintf(buf, max, "%s/%s (Macintosh; Intel Mac OS X) libuv/%s CUDA/%d.%d clang/%d.%d.%d", APP_NAME, APP_VERSION, uv_version_string(), cudaVersion / 1000, cudaVersion % 100, __clang_major__, __clang_minor__, __clang_patchlevel__);
#   else
    snprintf(buf, max, "%s/%s (Macintosh; Intel Mac OS X) libuv/%s clang/%d.%d.%d", APP_NAME, APP_VERSION, uv_version_string(), __clang_major__, __clang_minor__, __clang_patchlevel__);
#   endif

    return buf;
}


void Platform::init(const char *userAgent)
{
    m_userAgent = userAgent ? strdup(userAgent) : createUserAgent();
}


void Platform::release()
{
    delete [] m_userAgent;
}


void Platform::setProcessPriority(int priority)
{

}


void Platform::setThreadPriority(int priority)
{
    if (priority == -1) {
        return;
    }

    int prio = 19;
    switch (priority)
    {
    case 1:
        prio = 5;
        break;

    case 2:
        prio = 0;
        break;

    case 3:
        prio = -5;
        break;

    case 4:
        prio = -10;
        break;

    case 5:
        prio = -15;
        break;

    default:
        break;
    }

    setpriority(PRIO_PROCESS, 0, prio);
}

