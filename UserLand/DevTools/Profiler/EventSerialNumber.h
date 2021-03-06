/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */


#pragma once

#include <AK/Assertions.h>
#include <AK/NumericLimits.h>
#include <AK/Types.h>

namespace Profiler {

class EventSerialNumber {
public:
    constexpr EventSerialNumber() = default;

    void increment()
    {
        m_serial++;
    }

    size_t to_number() const
    {
        return m_serial;
    }

    static EventSerialNumber max_valid_serial()
    {
        return EventSerialNumber { NumericLimits<size_t>::max() };
    }

    bool operator==(EventSerialNumber const& rhs) const
    {
        return m_serial == rhs.m_serial;
    }

    bool operator<(EventSerialNumber const& rhs) const
    {
        return m_serial < rhs.m_serial;
    }

    bool operator>(EventSerialNumber const& rhs) const
    {
        return m_serial > rhs.m_serial;
    }

    bool operator<=(EventSerialNumber const& rhs) const
    {
        return m_serial <= rhs.m_serial;
    }

    bool operator>=(EventSerialNumber const& rhs) const
    {
        return m_serial >= rhs.m_serial;
    }

private:
    size_t m_serial { 0 };

    constexpr explicit EventSerialNumber(size_t serial)
        : m_serial(serial)
    {
    }
};

}
