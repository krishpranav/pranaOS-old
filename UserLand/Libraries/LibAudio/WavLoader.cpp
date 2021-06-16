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

}