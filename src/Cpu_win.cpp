#include <windows.h>


#include "CpuImpl.h"
#include "Mem.h"

void CpuImpl::init()
{
#   ifdef XMRIG_NO_LIBCPUID
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);

    m_totalThreads = sysinfo.dwNumberOfProcessors;
#   endif

    initCommon();
}


void CpuImpl::setAffinity(int id, uint64_t mask)
{
    if (id == -1) {
        SetProcessAffinityMask(GetCurrentProcess(), mask);
    } else {
        Mem::ThreadBitSet threadAffinityMask = Mem::ThreadBitSet(mask);

        int threadCount = 0;

        for (size_t i = 0; i < m_totalThreads; i++) {
            if (threadAffinityMask.test(i)) {
                if (threadCount == id) {
                    SetThreadAffinityMask(GetCurrentThread(), 1ULL << i);
                    break;
                }

                threadCount++;
            }
        }
    }
}
