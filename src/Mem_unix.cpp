#include <cstdlib>
#include <sys/mman.h>


#if defined(XMRIG_ARM) && !defined(__clang__)
#   include "aligned_malloc.h"
#else
#   include <mm_malloc.h>
#endif


#include "crypto/CryptoNight.h"
#include "log/Log.h"
#include "Mem.h"


bool Mem::allocate(const Options* options)
{
    m_algo       = options->algo();
    m_threads    = options->threads();
    m_hashFactor = options->hashFactor();
    m_multiHashThreadMask = Mem::ThreadBitSet(options->multiHashThreadMask());
    m_memorySize = 0;

    size_t scratchPadSize;
    switch (m_algo)
    {
        case Options::ALGO_CRYPTONIGHT_LITE:
            scratchPadSize = MEMORY_LITE;
            break;
        case Options::ALGO_CRYPTONIGHT_HEAVY:
            scratchPadSize = MEMORY_HEAVY;
            break;
        case Options::ALGO_CRYPTONIGHT:
        default:
            scratchPadSize = MEMORY;
            break;
    }

    for (size_t i=0; i < m_threads; i++) {
        m_memorySize += sizeof(cryptonight_ctx);
        m_memorySize += scratchPadSize * getThreadHashFactor(i);
    }

    m_memorySize = m_memorySize - (m_memorySize % MEMORY) + MEMORY;

    if (!options->hugePages()) {
        m_memory = static_cast<uint8_t*>(_mm_malloc(m_memorySize, 16));
        return true;
    }

    m_flags |= HugepagesAvailable;

#   if defined(__APPLE__)
    m_memory = static_cast<uint8_t*>(mmap(0, m_memorySize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, VM_FLAGS_SUPERPAGE_SIZE_2MB, 0));
#   elif defined(__FreeBSD__)
    m_memory = static_cast<uint8_t*>(mmap(0, m_memorySize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_ALIGNED_SUPER | MAP_PREFAULT_READ, -1, 0));
#   else
    m_memory = static_cast<uint8_t*>(mmap(nullptr, m_memorySize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB | MAP_POPULATE, 0, 0));
#   endif
    if (m_memory == MAP_FAILED) {
        m_memory = static_cast<uint8_t*>(_mm_malloc(m_memorySize, 16));
        return true;
    }

    m_flags |= HugepagesEnabled;

    if (madvise(m_memory, m_memorySize, MADV_RANDOM | MADV_WILLNEED) != 0) {
        LOG_ERR("madvise failed");
    }

    if (mlock(m_memory, m_memorySize) == 0) {
        m_flags |= Lock;
    }

    return true;
}


void Mem::release()
{
    if (m_flags & HugepagesEnabled) {
        if (m_flags & Lock) {
            munlock(m_memory, m_memorySize);
        }

        munmap(m_memory, m_memorySize);
    }
    else {
        _mm_free(m_memory);
    }
}
