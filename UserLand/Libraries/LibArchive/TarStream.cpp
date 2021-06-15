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


}