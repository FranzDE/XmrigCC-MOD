#ifndef __CPU_IMPL_H__
#define __CPU_IMPL_H__


#include <cstdint>
#include <vector>

#include "Options.h"

class CpuImpl
{
public:
    static CpuImpl& instance();
    CpuImpl();
    void init();

    void optimizeParameters(size_t& threadsCount, size_t& hashFactor, Options::Algo algo,
                            size_t maxCpuUsage, bool safeMode);
    void setAffinity(int id, uint64_t mask);

    bool hasAES();
    bool isX64();
    const char *brand() { return m_brand; }
    size_t l2()            { return m_l2_cache; }
    size_t l3()            { return m_l3_cache; }
    size_t cores()         { return m_totalCores; }
    size_t sockets()       { return m_sockets; }
    size_t threads()       { return m_totalThreads; }
    size_t availableCache();

private:
    void initCommon();

    bool m_l2_exclusive;
    char m_brand[64];
    int m_flags;
    size_t m_l2_cache;
    size_t m_l3_cache;
    size_t m_sockets;
    size_t m_totalCores;
    size_t m_totalThreads;
};

#endif /* __CPU_IMPL_H__ */
