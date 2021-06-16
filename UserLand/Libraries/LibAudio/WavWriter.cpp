/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <LibAudio/WavWriter.h>

namespace Audio {

WavWriter::WavWriter(const StringView& path, int sample_rate, int num_channels, int bits_per_sample)
    : m_sample_rate(sample_rate)
    , m_num_channels(num_channels)
    , m_bits_per_sample(bits_per_sample)
{
    set_file(path);
}

WavWriter::WavWriter(int sample_rate, int num_channels, int bits_per_sample)
    : m_sample_rate(sample_rate)
    , m_num_channels(num_channels)
    , m_bits_per_sample(bits_per_sample)
{
}

WavWriter::~WavWriter()
{
    if (!m_finalized)
        finalize();
}

void WavWriter::set_file(const StringView& path)
{
    m_file = Core::File::construct(path);
    if (!m_file->open(Core::OpenMode::ReadWrite)) {
        m_error_string = String::formatted("Can't open file: {}", m_file->error_string());
        return;
    }
    m_file->seek(44);
    m_finalized = false;
}

}