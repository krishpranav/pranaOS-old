
#pragma once

#include <AKF/ByteBuffer.h>
#include <AKF/Optional.h>
#include <AKF/String.h>
#include <AKF/StringView.h>

namespace AKF {

constexpr u8 decode_hex_digit(char digit)
{
    if (digit >= '0' && digit <= '9')
        return digit - '0';
    if (digit >= 'a' && digit <= 'f')
        return 10 + (digit - 'a');
    if (digit >= 'A' && digit <= 'F')
        return 10 + (digit - 'A');
    return 255;
}

Optional<ByteBuffer> decode_hex(const StringView&);

String encode_hex(ReadonlyBytes);

}

using AKF::decode_hex;
using AKF::decode_hex_digit;
using AKF::encode_hex;
