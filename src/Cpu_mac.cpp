#include <pthread.h>
#include <sched.h>
#include <unistd.h>


#include "Cpu.h"
#include "CpuImpl.h"

void CpuImpl::init()
{
#   ifdef XMRIG_NO_LIBCPUID
    m_totalThreads = sysconf(_SC_NPROCESSORS_CONF);
#   endif

    initCommon();
}


void CpuImpl::setAffinity(int id, uint64_t mask)
{
}
