#pragma once

// includes
#include <AK/CircularQueue.h>
#include <AK/DoublyLinkedList.h>
#include <AK/Types.h>
#include <Kernel/API/KeyCode.h>
#include <Kernel/Devices/HID/I8042Controller.h>
#include <Kernel/Devices/HID/KeyboardDevice.h>
#include <Kernel/Interrupts/IRQHandler.h>
#include <Kernel/Random.h>
#include <LibKeyboard/CharacterMap.h>

namespace Kernel {

class PS2KeyboardDevice final : public IRQHandler
    , public KeyboardDevice
    , public I8042Device {
    AK_MAKE_ETERNAL
public:
    static RefPtr<PS2KeyboardDevice> try_to_initialize(const I8042Controller&);
    virtual ~PS2KeyboardDevice() override;
    bool initialize();

    virtual const char* purpose() const override { return class_name(); }

    // ^I8042Device
    virtual void irq_handle_byte_read(u8 byte) override;
    virtual void enable_interrupts() override
    {
        enable_irq();
    }

private:
    explicit PS2KeyboardDevice(const I8042Controller&);

    // ^IRQHandler
    virtual void handle_irq(const RegisterState&) override;

    // ^CharacterDevice
    virtual const char* class_name() const override { return "KeyboardDevice"; }
};

}
