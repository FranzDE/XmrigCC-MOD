#include <cmath>
#include <cstring>
#include <algorithm>
#include <memory>

#include <libcpuid.h>
#include <iostream>

#include "Cpu.h"
#include "CpuImpl.h"

void CpuImpl::initCommon()
{
    struct cpu_raw_data_t raw = { 0 };
    struct cpu_id_t data = { 0 };

    cpuid_get_raw_data(&raw);
    cpu_identify(&raw, &data);

    strncpy(m_brand, data.brand_str, sizeof(m_brand) - 1);

    m_totalThreads = data.total_logical_cpus;
    m_sockets      = m_totalThreads / data.num_logical_cpus;

    if (m_sockets == 0) {
        m_sockets = 1;
    }

    m_totalCores = data.num_cores * m_sockets;
    m_l3_cache = data.l3_cache > 0 ? data.l3_cache * m_sockets : 0;

    // Workaround for AMD CPUs https://github.com/anrieff/libcpuid/issues/97
    if (data.vendor == VENDOR_AMD && data.ext_family >= 0x15 && data.ext_family < 0x17) {
        m_l2_cache = data.l2_cache * (m_totalCores / 2) * m_sockets;
        m_l2_exclusive = true;
    }
    // Workaround for Intel Pentium Dual-Core, Core Duo, Core 2 Duo, Core 2 Quad and their Xeon homologue
    // These processors have L2 cache shared by 2 cores.
    else if (data.vendor == VENDOR_INTEL && data.ext_family == 0x06 && (data.ext_model == 0x0E || data.ext_model == 0x0F || data.ext_model == 0x17)) {
        int l2_count_per_socket = m_totalCores > 1 ? m_totalCores / 2 : 1;
        m_l2_cache = data.l2_cache > 0 ? data.l2_cache * l2_count_per_socket * m_sockets : 0;
    }
    else{
        m_l2_cache = data.l2_cache > 0 ? data.l2_cache * m_totalCores * m_sockets : 0;
    }

#   if defined(__x86_64__) || defined(_M_AMD64)
    m_flags |= Cpu::X86_64;
#   endif

    if (data.flags[CPU_FEATURE_AES]) {
        m_flags |= Cpu::AES;
    }

    if (data.flags[CPU_FEATURE_BMI2]) {
        m_flags |= Cpu::BMI2;
    }
}
