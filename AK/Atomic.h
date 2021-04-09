#pragma once

#include <AK/Platform.h>
#include <AK/Types.h>

namespace AK {


static inline void atomic_signal_fence(MemoryOrder order) noexcept
{
    return __atomic_signal_fence(order);
}

static inline void atomic_thread_fence(MemoryOrder order) noexcept
{
    return __atomic_thread_fence(order);
}

static inline void full_memory_barrier() noexcept
{
    atomic_signal_fence(AK::MemoryOrder::memory_order_acq_rel);
    atomic_thread_fence(AK::MemoryOrder::memory_order_acq_rel);
}

template<typename T>

}
