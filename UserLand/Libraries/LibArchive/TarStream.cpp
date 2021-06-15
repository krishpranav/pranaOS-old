/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


// includes
#include <LibArchive/TarStream.h>
#include <string.h>

namespace Archive {
TarFileStream::TarFileStream(TarInputStream& tar_stream)
    : m_tar_stream(tar_stream)
    , m_generation(tar_stream.m_generation)
{    
}

size_t TarFileStream::read(Bytes bytes)
{
    VERIFY(m_tar_stream.m_generation == m_generation);

    if (has_any_error())
        return 0;

    auto to_read = min(bytes.size(), m_tar_stream.header().size() - m_tar_stream.m_file_offset);

    auto nread = m_tar_stream.m_stream.read(bytes.trim(to_read));
    m_tar_stream.m_file_offset += nread;
    return nread;
}


bool TarFileStream::unreliable_eof() const
{
    VERIFY(m_tar_stream.m_generation == m_generation);

    return m_tar_stream.m_stream.unreliable_eof()
        || m_tar_stream.m_file_offset >= m_tar_stream.header().size();
}
}