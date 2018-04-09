#include <string.h>
#include <uv.h>


#include "Platform.h"


char *Platform::m_defaultConfigName = nullptr;
char *Platform::m_userAgent         = nullptr;


const char *Platform::defaultConfigName()
{
    size_t size = 520;

    if (m_defaultConfigName == nullptr) {
        m_defaultConfigName = new char[size];
    }

    if (uv_exepath(m_defaultConfigName, &size) < 0) {
        return nullptr;
    }

    if (size < 500) {
#       ifdef WIN32
        char *p = strrchr(m_defaultConfigName, '\\');
#       else
        char *p = strrchr(m_defaultConfigName, '/');
#       endif

        if (p) {
#ifdef XMRIG_CC_SERVER
            strcpy(p + 1, "config_cc.json");
#else
            strcpy(p + 1, "config.json");
#endif
            return m_defaultConfigName;
        }
    }

    return nullptr;
}
