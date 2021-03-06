#pragma once

// includes
#include <AK/Function.h>
#include <AK/InlineLinkedList.h>
#include <AK/NonnullRefPtr.h>
#include <AK/OwnPtr.h>
#include <AK/RefCounted.h>
#include <AK/Time.h>
#include <Kernel/Time/TimeManagement.h>

namespace Kernel {

TYPEDEF_DISTINCT_ORDERED_ID(u64, TimerId);

class Timer : public RefCounted<Timer>
    , public InlineLinkedListNode<Timer> {
    friend class TimerQueue;
    friend class InlineLinkedListNode<Timer>;

public:
    Timer(clockid_t clock_id, Time expires, Function<void()>&& callback)
        : m_clock_id(clock_id)
        , m_expires(expires)
        , m_callback(move(callback))
    {
    }
    ~Timer()
    {
        VERIFY(!is_queued());
    }

    Time remaining() const;

private:
    TimerId m_id;
    clockid_t m_clock_id;
    Time m_expires;
    Time m_remaining {};
    Function<void()> m_callback;
    Timer* m_next { nullptr };
    Timer* m_prev { nullptr };
    Atomic<bool, AK::MemoryOrder::memory_order_relaxed> m_queued { false };

    bool operator<(const Timer& rhs) const
    {
        return m_expires < rhs.m_expires;
    }
    bool operator>(const Timer& rhs) const
    {
        return m_expires > rhs.m_expires;
    }
    bool operator==(const Timer& rhs) const
    {
        return m_id == rhs.m_id;
    }
    bool is_queued() const { return m_queued; }
    void set_queued(bool queued) { m_queued = queued; }
    Time now(bool) const;
};

class TimerQueue {
    friend class Timer;

public:
    TimerQueue();
    static TimerQueue& the();

    TimerId add_timer(NonnullRefPtr<Timer>&&);
    RefPtr<Timer> add_timer_without_id(clockid_t, const Time&, Function<void()>&&);
    TimerId add_timer(clockid_t, const Time& timeout, Function<void()>&& callback);
    bool cancel_timer(TimerId id);
    bool cancel_timer(Timer&);
    bool cancel_timer(NonnullRefPtr<Timer>&& timer)
    {
        return cancel_timer(*move(timer));
    }
    void fire();

private:
    struct Queue {
        InlineLinkedList<Timer> list;
        Time next_timer_due {};
    };
    void remove_timer_locked(Queue&, Timer&);
    void update_next_timer_due(Queue&);
    void add_timer_locked(NonnullRefPtr<Timer>);

    Queue& queue_for_timer(Timer& timer)
    {
        switch (timer.m_clock_id) {
        case CLOCK_MONOTONIC:
        case CLOCK_MONOTONIC_COARSE:
        case CLOCK_MONOTONIC_RAW:
            return m_timer_queue_monotonic;
        case CLOCK_REALTIME:
        case CLOCK_REALTIME_COARSE:
            return m_timer_queue_realtime;
        default:
            VERIFY_NOT_REACHED();
        }
    }

    u64 m_timer_id_count { 0 };
    u64 m_ticks_per_second { 0 };
    Queue m_timer_queue_monotonic;
    Queue m_timer_queue_realtime;
    InlineLinkedList<Timer> m_timers_executing;
};

}
