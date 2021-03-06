#pragma once

// includes
#include <AK/CircularQueue.h>
#include <AK/Vector.h>
#include <Kernel/Devices/CharacterDevice.h>

class Console final : public Kernel::CharacterDevice {
    AK_MAKE_ETERNAL
public:
    static Console& the();
    static void initialize();
    static bool is_initialized();

    Console();
    virtual ~Console() override;

    // ^CharacterDevice
    virtual bool can_read(const Kernel::FileDescription&, size_t) const override;
    virtual bool can_write(const Kernel::FileDescription&, size_t) const override { return true; }
    virtual Kernel::KResultOr<size_t> read(FileDescription&, u64, Kernel::UserOrKernelBuffer&, size_t) override;
    virtual Kernel::KResultOr<size_t> write(FileDescription&, u64, const Kernel::UserOrKernelBuffer&, size_t) override;
    virtual const char* class_name() const override { return "Console"; }

    void put_char(char);

    const CircularQueue<char, 16384>& logbuffer() const { return m_logbuffer; }

    // ^Device
    virtual mode_t required_mode() const override { return 0666; }
    virtual String device_name() const override { return "console"; }

private:
    CircularQueue<char, 16384> m_logbuffer;
};
