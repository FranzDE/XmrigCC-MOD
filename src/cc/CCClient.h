#ifndef __CC_CLIENT_H__
#define __CC_CLIENT_H__

#ifndef XMRIG_NO_CC

#include <uv.h>
#include <chrono>
#include <ctime>
#include <3rdparty/cpp-httplib/httplib.h>
#include "Options.h"
#include "ClientStatus.h"

class Hashrate;
class NetworkState;

class CCClient
{
public:
    CCClient(Options *options, uv_async_t* async);
    ~CCClient();

    static void updateHashrate(const Hashrate *hashrate);
    static void updateNetworkState(const NetworkState &results);

private:

    static void publishClientStatusReport();
    static void updateConfig();
    static void publishConfig();
    static std::shared_ptr<httplib::Response> performRequest(const std::string& requestUrl,
                                                             const std::string& requestBuffer,
                                                             const std::string& operation);

    static void onThreadStarted(void *handle);
    static void onReport(uv_timer_t *handle);

    const Options* m_options;

    static CCClient* m_self;
    static uv_mutex_t m_mutex;

    ClientStatus m_clientStatus;

    std::string m_authorization;

    std::chrono::time_point<std::chrono::system_clock> m_startTime;

    uv_async_t* m_async;
    uv_timer_t m_timer;
    uv_loop_t m_client_loop;
    uv_thread_t m_thread;

    static void refreshUptime();
};

#endif
#endif /* __CC_CLIENT_H__ */
