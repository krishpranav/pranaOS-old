/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <AK/Atomic.h>
#include <AK/Debug.h>
#include <AK/String.h>
#include <LibAudio/Buffer.h>

namespace Audio {

u16 pcm_bits_per_sample(PcmSampleFormat format)
{
    switch (format) {
    case Uint8:
        return 8;
    case Int16:
        return 16;
    case Int24:
        return 24;
    case Float32:
        return 32;
    case Float64:
        return 64;
    default:
        VERIFY_NOT_REACHED();
    }
}

}