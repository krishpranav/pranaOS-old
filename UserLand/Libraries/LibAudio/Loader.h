/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <AK/ByteBuffer.h>
#include <AK/RefCounted.h>
#include <AK/RefPtr.h>
#include <AK/StringView.h>
#include <LibAudio/Buffer.h>
#include <LibCore/File.h>


namespace Audio {


class LoaderPlugin {
public:
    virtual ~LoaderPlugin() { }

    virtual bool sniff() = 0;

    virtual bool has_error() { return false; }
    virtual const char* error_string() { return ""; }

    virtual RefPtr<Buffer> get_more_samples(size_t max_bytes_to_read_from_input = 128 * KiB) = 0;

    virtual void reset() = 0;

    virtual void seek(const int sample_index) = 0;

    virtual int loaded_samples() = 0;
    virtual int total_samples() = 0;
    virtual u32 sample_rate() = 0;
    virtual u16 num_channels() = 0;
    virtual PcmSampleFormat pcm_format() = 0;
    virtual RefPtr<Core::File> file() = 0;
};


class Loader : public RefCounted<Loader> {
public:
    static NonnullRefPtr<Loader> create(const StringView& path) { return adopt_ref(*new Loader(path)); }
    static NonnullRefPtr<Loader> create(const ByteBuffer& buffer) { return adopt_ref(*new Loader(buffer)); }

    bool has_error() const { return m_plugin ? m_plugin->has_error() : true; }
    const char* error_string() const { return m_plugin ? m_plugin->error_string() : "No loader plugin available"; }

    RefPtr<Buffer> get_more_samples(size_t max_bytes_to_read_from_input = 128 * KiB) const { return m_plugin ? m_plugin->get_more_samples(max_bytes_to_read_from_input) : nullptr; }


};

}