/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <AK/ByteBuffer.h>
#include <AK/MemoryStream.h>
#include <AK/String.h>
#include <AK/Types.h>
#include <AK/Vector.h>
#include <LibCore/AnonymousBuffer.h>
#include <string.h>

namespace Audio {


struct Frame {
    Frame()
        : left(0)
        , right(0)
    {
    }

    Frame(double left)
        : left(left)
        , right(left)
    {
    }

    Frame(double left, double right)
        : left(left)
        , right(right)
    {
    }

    void clip()
    {
        if (left > 1)
            left = 1;
        else if (left < -1)
            left = -1;

        if (right > 1)
            right = 1;
        else if (right < -1)
            right = -1;
    }

    void scale(int percent)
    {
        double pct = (double)percent / 100.0;
        left *= pct;
        right *= pct;
    }

    Frame& operator+=(const Frame& other)
    {
        left += other.left;
        right += other.right;
        return *this;
    }

    double left;
    double right;
};

enum PcmSampleFormat : u8 {
    Uint8,
    Int16,
    Int24,
    Float32,
    Float64,
};

u16 pcm_bits_per_sample(PcmSampleFormat format);
String sample_format_name(PcmSampleFormat format);


}