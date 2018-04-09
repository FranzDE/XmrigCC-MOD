#include <cmath>
#include <cstring>
#include <algorithm>
#include <memory>

#include <iostream>

#include "Cpu.h"
#include "CpuImpl.h"

CpuImpl& CpuImpl::instance()
{
    static CpuImpl cpu;
    return cpu;
}

CpuImpl::CpuImpl()
    : m_l2_exclusive(false)
    , m_brand{ 0 }
    , m_flags(0)
    , m_l2_cache(0)
    , m_l3_cache(0)
    , m_sockets(1)
    , m_totalCores(0)
    , m_totalThreads(0)
{
}

void CpuImpl::optimizeParameters(size_t& threadsCount, size_t& hashFactor,
                                 Options::Algo algo, size_t maxCpuUsage, bool safeMode)
{
    // limits hashfactor to maximum possible value defined by compiler flag
    hashFactor = std::min(hashFactor, algo == Options::ALGO_CRYPTONIGHT_HEAVY ? 3 : static_cast<size_t>(MAX_NUM_HASH_BLOCKS));

    if (!safeMode && threadsCount > 0 && hashFactor > 0)
    {
      // all parameters have been set manually and safe mode is off ... no optimization necessary
      return;
    }

    size_t cache = availableCache();
    size_t algoBlockSize;
    switch (algo) {
        case Options::ALGO_CRYPTONIGHT_LITE:
            algoBlockSize = 1024;
            break;
        case Options::ALGO_CRYPTONIGHT_HEAVY:
            algoBlockSize = 4096;
            break;
        case Options::ALGO_CRYPTONIGHT:
        default:
            algoBlockSize = 2048;
            break;
    }

    size_t maximumReasonableFactor = std::max(cache / algoBlockSize, static_cast<size_t>(1ul));
    size_t maximumReasonableThreadCount = std::min(maximumReasonableFactor, m_totalThreads);
    size_t maximumReasonableHashFactor = std::min(maximumReasonableFactor, algo == Options::ALGO_CRYPTONIGHT_HEAVY ? 3 : static_cast<size_t>(MAX_NUM_HASH_BLOCKS));

    if (safeMode) {
        if (threadsCount > maximumReasonableThreadCount) {
            threadsCount = maximumReasonableThreadCount;
        }
        if (hashFactor > maximumReasonableFactor / threadsCount) {
            hashFactor = std::min(maximumReasonableFactor / threadsCount, maximumReasonableHashFactor);
            hashFactor   = std::max(hashFactor, static_cast<size_t>(1));
        }
    }

    if (threadsCount == 0) {
        if (hashFactor == 0) {
            threadsCount = maximumReasonableThreadCount;
        }
        else {
            threadsCount = std::min(maximumReasonableThreadCount,
                                    maximumReasonableFactor / hashFactor);
        }
        if (maxCpuUsage < 100)
        {
            threadsCount = std::min(threadsCount, m_totalThreads * maxCpuUsage / 100);
        }
        threadsCount = std::max(threadsCount, static_cast<size_t>(1));
    }
    if (hashFactor == 0) {
        hashFactor = std::min(maximumReasonableHashFactor, maximumReasonableFactor / threadsCount);
        hashFactor   = std::max(hashFactor, static_cast<size_t>(1));
    }
}

bool CpuImpl::hasAES()
{
    return (m_flags & Cpu::AES) != 0;
}

bool CpuImpl::isX64()
{
    return (m_flags & Cpu::X86_64) != 0;
}

size_t CpuImpl::availableCache()
{
    size_t cache = 0;
    if (m_l3_cache) {
        cache = m_l2_exclusive ? (m_l2_cache + m_l3_cache) : m_l3_cache;
    }
    else {
        cache = m_l2_cache;
    }
    return cache;
}

void Cpu::init()
{
    CpuImpl::instance().init();
}

void Cpu::optimizeParameters(size_t& threadsCount, size_t& hashFactor, Options::Algo algo,
                               size_t maxCpuUsage, bool safeMode)
{
    CpuImpl::instance().optimizeParameters(threadsCount, hashFactor, algo, maxCpuUsage, safeMode);
}

void Cpu::setAffinity(int id, uint64_t mask)
{
    CpuImpl::instance().setAffinity(id, mask);
}

bool Cpu::hasAES()
{
    return CpuImpl::instance().hasAES();
}

bool Cpu::isX64()
{
    return CpuImpl::instance().isX64();
}

const char* Cpu::brand()
{
    return CpuImpl::instance().brand();
}

size_t Cpu::cores()
{
    return CpuImpl::instance().cores();
}

size_t Cpu::l2()
{
    return CpuImpl::instance().l2();
}

size_t Cpu::l3()
{
    return CpuImpl::instance().l3();
}

size_t Cpu::sockets()
{
    return CpuImpl::instance().sockets();
}

size_t Cpu::threads()
{
    return CpuImpl::instance().threads();
}

size_t Cpu::availableCache()
{
    return CpuImpl::instance().availableCache();
}
