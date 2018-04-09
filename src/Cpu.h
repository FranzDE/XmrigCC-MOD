#ifndef __CPU_H__
#define __CPU_H__

#include <cstdint>

#include "Options.h"

class Cpu
{
public:
    enum Flags {
        X86_64 = 1,
        AES    = 2,
        BMI2   = 4
    };

    static void init();

    static void optimizeParameters(size_t& threadsCount, size_t& hashFactor, Options::Algo algo,
                                    size_t maxCpuUsage, bool safeMode);

    static void setAffinity(int id, uint64_t mask);

    static bool hasAES();
    static bool isX64();
    static const char *brand();
    static size_t l2();
    static size_t l3();
    static size_t cores();
    static size_t sockets();
    static size_t threads();
    static size_t availableCache();
};


#endif /* __CPU_H__ */
