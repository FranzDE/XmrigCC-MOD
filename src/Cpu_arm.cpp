#include <string.h>


#include "Cpu.h"
#include "CpuImpl.h"

void CpuImpl::initCommon()
{
    memcpy(m_brand, "Unknown", 7);

#   if defined(XMRIG_ARMv8)
    m_flags |= Cpu::X86_64;
    m_flags |= Cpu::AES;
#   endif
}
