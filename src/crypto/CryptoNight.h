#ifndef __CRYPTONIGHT_H__
#define __CRYPTONIGHT_H__


#include <cstddef>
#include <cstdint>

#include "Options.h"

#define MEMORY      2097152 /* 2 MiB */
#define MEMORY_LITE 1048576 /* 1 MiB */
#define MEMORY_HEAVY 4194304 /* 4 MiB */

struct cryptonight_ctx {
    alignas(16) uint8_t state[MAX_NUM_HASH_BLOCKS][208]; // 208 instead of 200 to maintain aligned to 16 byte boundaries
    alignas(16) uint8_t* memory;
};


class Job;
class JobResult;

class CryptoNight
{
public:
    static bool init(int algo, bool aesni);

    static void hash(size_t factor, const uint8_t* input, size_t size, uint8_t* output, cryptonight_ctx* ctx);

private:
    static bool selfTest(int algo);
};


#endif /* __CRYPTONIGHT_H__ */
