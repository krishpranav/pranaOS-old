#pragma once

#include <AK/String.h>
#include <Kernel/KBuffer.h>
#include <stdarg.h>

namespace Kernel {

class KBufferBuilder {
public:
    using OutputType = KBuffer;

    explicit KBufferBuilder(bool can_expand = false);
    explicit KBufferBuilder(RefPtr<KBufferImpl>&, bool can_expand = false);
    KBufferBuilder(KBufferBuilder&&) = default;
    ~KBufferBuilder() = default;

    void append(const StringView&);
    void append(char);
    void append(const char*, int);

    void append_escaped_for_json(const StringView&);
    void append_bytes(ReadonlyBytes);

    template<typename... Parameters>
    void appendff(CheckedFormatString<Parameters...>&& fmtstr, const Parameters&... parameters)
    {
        // FIXME: This is really not the way to go about it, but vformat expects a
        //        StringBuilder. Why does this class exist anyways?
        append(String::formatted(fmtstr.view(), parameters...));
    }

    bool flush();
    OwnPtr<KBuffer> build();

private:
    bool check_expand(size_t);
    u8* insertion_ptr()
    {
        if (!m_buffer)
            return nullptr;
        return m_buffer->data() + m_size;
    }

    RefPtr<KBufferImpl> m_buffer;
    size_t m_size { 0 };
    bool m_can_expand { false };
};

}
