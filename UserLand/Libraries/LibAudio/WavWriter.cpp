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

}