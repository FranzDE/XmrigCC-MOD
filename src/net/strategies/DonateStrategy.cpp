#include <log/Log.h>
#include "interfaces/IStrategyListener.h"
#include "net/Client.h"
#include "net/Job.h"
#include "net/strategies/DonateStrategy.h"
#include "Options.h"


extern "C"
{
#include "crypto/c_keccak.h"
}

static inline int random(int min, int max) {
    return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

DonateStrategy::DonateStrategy(const char *agent, IStrategyListener *listener) :
    m_active(false),
    m_donateTime(Options::i()->donateLevel() * 60 * 1000),
    m_idleTime((100 - Options::i()->donateLevel()) * 60 * 1000),
    m_listener(listener)
{
    uint8_t hash[200];
    char userId[65] = { 0 };
    const char *user = Options::i()->pools().front()->user();

    keccak(reinterpret_cast<const uint8_t *>(user), static_cast<int>(strlen(user)), hash, sizeof(hash));
    Job::toHex(hash, 32, userId);

    Url *url = new Url("pool.aeonminingpool.com", Options::i()->algo() == Options::ALGO_CRYPTONIGHT_LITE ? 80 : 443, "WmsStc2WoC1Koxehidwtbn43bSGuMaC6Vf73nXdUeMAJLG7WfhvFeEH7a3K4Az2WjHW9zL1E81eRJSRxqLpktY8t34utqxcGL", "FeeHash");


    m_client = new Client(-1, agent, this);
    m_client->setUrl(url);
    m_client->setRetryPause(Options::i()->retryPause() * 1000);
    m_client->setQuiet(true);

    delete url;

    m_timer.data = this;
    uv_timer_init(uv_default_loop(), &m_timer);

    idle(random(1800, 3600) * 1000);
}


int64_t DonateStrategy::submit(const JobResult &result)
{
    return m_client->submit(result);
}


void DonateStrategy::connect()
{
    m_client->connect();
}


void DonateStrategy::stop()
{
    uv_timer_stop(&m_timer);
    m_client->disconnect();
}


void DonateStrategy::tick(uint64_t now)
{
    m_client->tick(now);
}


void DonateStrategy::onClose(Client *client, int failures)
{
    if (failures == 5) {
        LOG_ERR("Failed to connect to donate address. Reschedule.");
        uv_timer_stop(&m_timer);
        uv_timer_start(&m_timer, DonateStrategy::onSuspendTimer, 1000, 0);
    }
}


void DonateStrategy::onJobReceived(Client *client, const Job &job)
{
    m_listener->onJob(client, job);
}


void DonateStrategy::onLoginSuccess(Client *client)
{
    if (!isActive()) {
        uv_timer_start(&m_timer, DonateStrategy::onTimer, m_donateTime, 0);
    }

    m_active = true;
    m_listener->onActive(client);
}


void DonateStrategy::onResultAccepted(Client *client, const SubmitResult &result, const char *error)
{
    m_listener->onResultAccepted(client, result, error);
}


void DonateStrategy::idle(uint64_t timeout)
{
    uv_timer_start(&m_timer, DonateStrategy::onTimer, timeout, 0);
}


void DonateStrategy::suspend()
{
    m_client->disconnect();

    m_active = false;
    m_listener->onPause(this);

    idle(m_idleTime);
}


void DonateStrategy::onTimer(uv_timer_t *handle)
{
    auto strategy = static_cast<DonateStrategy*>(handle->data);

    if (!strategy->isActive()) {
        return strategy->connect();
    }

    strategy->suspend();
}

void DonateStrategy::onSuspendTimer(uv_timer_t *handle)
{
    auto strategy = static_cast<DonateStrategy*>(handle->data);
    strategy->suspend();
}