/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <AK/Span.h>
#include <AK/Stream.h>
#include <LibArchive/Tar.h>

namespace Archive {

class TarInputStream;

class TarFileStream : public InputStream {
public:
    size_t read(Bytes) override;
    bool unreliable_eof() const override;

    bool read_or_error(Bytes) override;
    bool discard_or_error(size_t count) override;

    
}

}