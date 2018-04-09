#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#include "interfaces/ILogBackend.h"
#include "log/Log.h"


Log *Log::m_self = nullptr;


void Log::message(Log::Level level, const char* fmt, ...)
{
    va_list args;
    va_list copy;
    va_start(args, fmt);

    for (ILogBackend *backend : m_backends) {
        va_copy(copy, args);
        backend->message(level, fmt, copy);
        va_end(copy);
    }
}


void Log::text(const char* fmt, ...)
{
    va_list args;
    va_list copy;
    va_start(args, fmt);

    for (ILogBackend *backend : m_backends) {
        va_copy(copy, args);
        backend->text(fmt, copy);
        va_end(copy);
    }

    va_end(args);
}


Log::~Log()
{
    for (auto backend : m_backends) {
        delete backend;
    }
}
