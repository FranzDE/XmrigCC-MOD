#ifndef __CLIENT_H__
#define __CLIENT_H__


#include <map>
#include <uv.h>
#include <memory>


#include "net/Job.h"
#include "net/SubmitResult.h"
#include "net/Url.h"
#include "rapidjson/fwd.h"

#ifdef __cplusplus
extern "C"
{
#endif

#include "net.h"

#ifndef XMRIG_NO_TLS
#include "tls.h"
#endif

#ifdef __cplusplus
}
#endif

class IClientListener;
class JobResult;


class Client
{
public:
    constexpr static int kResponseTimeout  = 20 * 1000;
    constexpr static int kKeepAliveTimeout = 60 * 1000;

    Client(int id, const char *agent, IClientListener *listener);
    ~Client();

    int64_t submit(const JobResult &result);
    void connect();
    void connect(const Url *url);
    void disconnect();
    void setUrl(const Url *url);
    void tick(uint64_t now);

    inline const char *host() const          { return m_url.host(); }
    inline const Job &job() const            { return m_job; }
    inline int id() const                    { return m_id; }
    inline uint16_t port() const             { return m_url.port(); }
    inline void setQuiet(bool quiet)         { m_quiet = quiet; }
    inline void setRetryPause(int ms)        { m_retryPause = ms; }

private:
    bool isCriticalError(const char *message);
    bool parseJob(const rapidjson::Value &params, int *code);
    bool parseLogin(const rapidjson::Value &result, int *code);
    int64_t send(size_t size);
    void close();
    void login();
    void parse(char *line, size_t len);
    void parseExtensions(const rapidjson::Value &value);
    void parseNotification(const char *method, const rapidjson::Value &params, const rapidjson::Value &error);
    void parseResponse(int64_t id, const rapidjson::Value &result, const rapidjson::Value &error);
    void ping();
    void reconnect();
    void startTimeout();

    static void onRead(net_t *net, size_t read, char *buf);
    static void onConnect(net_t *net);
    static void onError(net_t *net, int err, char *errStr);
    static void onClose(net_t *net);

    static inline Client *getClient(void *data) { return static_cast<Client*>(data); }

    bool m_quiet;
    bool m_nicehash;
    char m_buf[2048];
    char m_rpcId[64];
    char m_sendBuf[768];
    const char *m_agent;
    IClientListener *m_listener;
    int m_id;
    int m_retryPause;
    int64_t m_failures;
    int64_t m_jobs;
    Job m_job;
    size_t m_recvBufPos;
    static int64_t m_sequence;
    std::map<int64_t, SubmitResult> m_results;
    uint64_t m_expire;
    Url m_url;
    uv_buf_t m_recvBuf;

    net_t* m_net;

#   ifndef XMRIG_PROXY_PROJECT
    uv_timer_t m_keepAliveTimer;
#   endif

};


#endif /* __CLIENT_H__ */
