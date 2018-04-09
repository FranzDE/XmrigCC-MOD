#include <memory.h>


#include "crypto/CryptoNight.h"
#include "Mem.h"


int Mem::m_algo          = 0;
int Mem::m_flags         = 0;
size_t Mem::m_hashFactor = 1;
size_t Mem::m_threads    = 0;
size_t Mem::m_memorySize = 0;
alignas(16) uint8_t *Mem::m_memory = nullptr;
Mem::ThreadBitSet Mem::m_multiHashThreadMask = Mem::ThreadBitSet(-1L);

cryptonight_ctx *Mem::create(int threadId)
{
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

    size_t offset = 0;
    for (int i=0; i < threadId; i++) {
        offset += sizeof(cryptonight_ctx);
        offset += scratchPadSize * getThreadHashFactor(i);
    }

    auto* ctx = reinterpret_cast<cryptonight_ctx *>(&m_memory[offset]);

    size_t memOffset = offset+sizeof(cryptonight_ctx);

    ctx->memory = &m_memory[memOffset];

    return ctx;
}
