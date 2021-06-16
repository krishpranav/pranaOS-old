/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


// includes
#include <AK/Debug.h>
#include <AK/NumericLimits.h>
#include <AK/OwnPtr.h>
#include <LibAudio/Buffer.h>
#include <LibAudio/WavLoader.h>
#include <LibCore/File.h>
#include <LibCore/FileStream.h>

namespace Audio {

static constexpr size_t maximum_wav_size = 1 * GiB;

WavLoaderPlugin::WavLoaderPlugin(const StringView& path)
    : m_file(Core::File::construct(path))
{
    if (!m_file->open(Core::OpenMode::ReadOnly)) {
        m_error_string = String::formatted("Can't open file: {}", m_file->error_string());
        return;
    }
    m_stream = make<Core::InputFileStream>(*m_file);

    valid = parse_header();
    if (!valid)
        return;

    m_resampler = make<ResampleHelper>(m_sample_rate, 44100);
}

WavLoaderPlugin::WavLoaderPlugin(const ByteBuffer& buffer)
{
    m_stream = make<InputMemoryStream>(buffer);
    if (!m_stream) {
        m_error_string = String::formatted("Can't open memory stream");
        return;
    }
    m_memory_stream = static_cast<InputMemoryStream*>(m_stream.ptr());

    valid = parse_header();
    if (!valid)
        return;

    m_resampler = make<ResampleHelper>(m_sample_rate, 44100);
}

RefPtr<Buffer> WavLoaderPlugin::get_more_samples(size_t max_bytes_to_read_from_input)
{
    if (!m_stream)
        return nullptr;

    size_t bytes_per_sample = (m_num_channels * (pcm_bits_per_sample(m_sample_format) / 8));

    // Might truncate if not evenly divisible
    size_t samples_to_read = static_cast<int>(max_bytes_to_read_from_input) / bytes_per_sample;
    size_t bytes_to_read = samples_to_read * bytes_per_sample;

    dbgln_if(AWAVLOADER_DEBUG, "Read {} bytes ({} samples) WAV with num_channels {} sample rate {}, "
                               "bits per sample {}, sample format {}",
        bytes_to_read, samples_to_read, m_num_channels, m_sample_rate,
        pcm_bits_per_sample(m_sample_format), sample_format_name(m_sample_format));

    ByteBuffer sample_data = ByteBuffer::create_zeroed(bytes_to_read);
    m_stream->read_or_error(sample_data.bytes());
    if (m_stream->handle_any_error()) {
        return nullptr;
    }

    RefPtr<Buffer> buffer = Buffer::from_pcm_data(
        sample_data.bytes(),
        *m_resampler,
        m_num_channels,
        m_sample_format);

    // m_loaded_samples should contain the amount of actually loaded samples
    m_loaded_samples += samples_to_read;
    m_loaded_samples = min(m_total_samples, m_loaded_samples);
    return buffer;
}

void WavLoaderPlugin::seek(const int sample_index)
{
    dbgln_if(AWAVLOADER_DEBUG, "seek sample_index {}", sample_index);
    if (sample_index < 0 || sample_index >= m_total_samples)
        return;

    m_loaded_samples = sample_index;
    size_t byte_position = m_byte_offset_of_data_samples + sample_index * m_num_channels * (pcm_bits_per_sample(m_sample_format) / 8);

    // AK::InputStream does not define seek.
    if (m_file) {
        m_file->seek(byte_position);
    } else {
        m_memory_stream->seek(byte_position);
    }
}

}