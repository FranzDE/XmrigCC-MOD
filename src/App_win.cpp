#include <winsock2.h>
#include <windows.h>


#include "App.h"
#include "Options.h"
#include "Cpu.h"


void App::background()
{
    if (m_options->affinity() != -1L) {
        Cpu::setAffinity(-1, m_options->affinity());
    }

    if (!m_options->background()) {
        return;
    }

    HWND hcon = GetConsoleWindow();
    if (hcon) {
        ShowWindow(hcon, SW_HIDE);
    } else {
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
        CloseHandle(h);
        FreeConsole();
    }
}
