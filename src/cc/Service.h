#ifndef __SERVICE_H__
#define __SERVICE_H__

#define CONTENT_TYPE_HTML "text/html"
#define CONTENT_TYPE_JSON "application/json"

#include <string>
#include <uv.h>
#include <microhttpd.h>
#include <map>
#include "Options.h"
#include "ClientStatus.h"
#include "ControlCommand.h"

class Service
{
public:
    static bool start();
    static void release();

    static unsigned handleGET(const Options* options, const std::string& url, const std::string& clientId, std::string& resp);
    static unsigned handlePOST(const Options* options, const std::string& url, const std::string& clientIp, const std::string& clientId, const std::string& data, std::string& resp);

private:
    static unsigned getClientConfig(const Options* options, const std::string& clientId, std::string& resp);
    static unsigned getClientCommand(const std::string& clientId, std::string& resp);
    static unsigned getClientStatusList(std::string& resp);
    static unsigned getAdminPage(const Options* options, std::string& resp);

    static unsigned setClientStatus(const std::string& clientIp, const std::string& clientId, const std::string& data, std::string& resp);
    static unsigned setClientCommand(const std::string& clientId, const std::string& data, std::string& resp);
    static unsigned setClientConfig(const Options* options, const std::string &clientId, const std::string &data, std::string &resp);

    static std::string getClientConfigFileName(const Options *options, const std::string &clientId);

private:
    static int m_currentServerTime;

    static std::map<std::string, ClientStatus> m_clientStatus;
    static std::map<std::string, ControlCommand> m_clientCommand;

    static uv_mutex_t m_mutex;

};

#endif /* __SERVICE_H__ */
