#pragma once

// includes
#include <AK/NonnullRefPtr.h>
#include <Kernel/RTC.h>
#include <Kernel/Time/HardwareTimer.h>

namespace Kernel {
class RealTimeClock final : public HardwareTimer<IRQHandler> {
public:
    static NonnullRefPtr<RealTimeClock> create(Function<void(const RegisterState&)> callback);
    virtual HardwareTimerType timer_type() const override { return HardwareTimerType::RTC; }
    virtual const char* model() const override { return "Real Time Clock"; }
    virtual size_t ticks_per_second() const override;

    virtual bool is_periodic() const override { return true; }
    virtual bool is_periodic_capable() const override { return true; }
    virtual void set_periodic() override { }
    virtual void set_non_periodic() override { }
    virtual void disable() override { }

    virtual void reset_to_default_ticks_per_second() override;
    virtual bool try_to_set_frequency(size_t frequency) override;
    virtual bool is_capable_of_frequency(size_t frequency) const override;
    virtual size_t calculate_nearest_possible_frequency(size_t frequency) const override;

private:
    explicit RealTimeClock(Function<void(const RegisterState&)> callback);
    virtual void handle_irq(const RegisterState&) override;
};
}
