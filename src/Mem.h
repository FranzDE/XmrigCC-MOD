#ifndef __MEM_H__
#define __MEM_H__


#include <stddef.h>
#include <stdint.h>
#include <bitset>

#include "Options.h"

struct cryptonight_ctx;


class Mem
{
public:
    typedef std::bitset<128> ThreadBitSet;
    enum Flags {
        HugepagesAvailable = 1,
        HugepagesEnabled   = 2,
        Lock               = 4
    };

    static bool allocate(const Options* options);
    static cryptonight_ctx *create(int threadId);
    static void release();

    static inline size_t hashFactor()         { return m_hashFactor; }
    static inline size_t getThreadHashFactor(int threadId)
    {
        return (m_multiHashThreadMask.all() ||
                m_multiHashThreadMask.test(threadId)) ? m_hashFactor : 1;
    }
    static inline bool isHugepagesAvailable() { return (m_flags & HugepagesAvailable) != 0; }
    static inline bool isHugepagesEnabled()   { return (m_flags & HugepagesEnabled) != 0; }
    static inline int flags()                 { return m_flags; }
    static inline size_t threads()            { return m_threads; }

private:
    static size_t m_hashFactor;
    static size_t m_threads;
    static int m_algo;
    static int m_flags;
    static ThreadBitSet m_multiHashThreadMask;
    static size_t m_memorySize;
    alignas(16) static uint8_t *m_memory;
};


#endif /* __MEM_H__ */
