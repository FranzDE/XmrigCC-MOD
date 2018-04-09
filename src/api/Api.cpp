#include <string.h>


#include <cstring>

#include "api/Api.h"
#include "api/ApiState.h"


ApiState *Api::m_state = nullptr;
uv_mutex_t Api::m_mutex;


bool Api::start()
{
    uv_mutex_init(&m_mutex);
    m_state = new ApiState();

    return true;
}


void Api::release()
{
    delete m_state;
}


char *Api::get(const char *url, int *status)
{
    if (!m_state) {
        return nullptr;
    }

    uv_mutex_lock(&m_mutex);
    char *buf = m_state->get(url, status);
    uv_mutex_unlock(&m_mutex);

    return buf;
}


void Api::tick(const Hashrate *hashrate)
{
    if (!m_state) {
        return;
    }

    uv_mutex_lock(&m_mutex);
    m_state->tick(hashrate);
    uv_mutex_unlock(&m_mutex);
}


void Api::tick(const NetworkState &network)
{
    if (!m_state) {
        return;
    }

    uv_mutex_lock(&m_mutex);
    m_state->tick(network);
    uv_mutex_unlock(&m_mutex);
}
