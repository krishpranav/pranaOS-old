/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <LibArchive/Zip.h>

namespace Archive {

bool Zip::find_end_of_central_directory_offset(const ReadonlyBytes& buffer, size_t& offset)
{
    for (size_t backwards_offset = 0; backwards_offset <= UINT16_MAX; backwards_offset++) // the file may have a trailing comment of an arbitrary 16 bit length
    {
        if (buffer.size() < (sizeof(EndOfCentralDirectory) - sizeof(u8*)) + backwards_offset)
            return false;

        auto signature_offset = (buffer.size() - (sizeof(EndOfCentralDirectory) - sizeof(u8*)) - backwards_offset);
        if (memcmp(buffer.data() + signature_offset, end_of_central_directory_signature, sizeof(end_of_central_directory_signature)) == 0) {
            offset = signature_offset;
            return true;
        }
    }
    return false;
}

}