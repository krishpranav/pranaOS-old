#pragma once

// includes
#include <AK/CircularQueue.h>
#include <Kernel/API/MousePacket.h>
#include <Kernel/Devices/HID/I8042Controller.h>
#include <Kernel/Devices/HID/PS2MouseDevice.h>
#include <Kernel/Interrupts/IRQHandler.h>
#include <Kernel/Random.h>

namespace Kernel {

class VMWareMouseDevice final : public PS2MouseDevice {
public:
    static RefPtr<VMWareMouseDevice> try_to_initialize(const I8042Controller&);
    virtual ~VMWareMouseDevice() override;

    // ^I8042Device
    virtual void irq_handle_byte_read(u8 byte) override;

private:
    explicit VMWareMouseDevice(const I8042Controller&);
};

}
