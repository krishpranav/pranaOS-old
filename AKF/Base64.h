#pragma once

#include <AK/ByteBuffer.h>
#include <AK/String.h>
#include <AK/StringView.h>

namespace AK {

size_t calculate_base64_decoded_length(const StringView&);

size_t calculate_base64_encoded_length(ReadonlyBytes);

ByteBuffer decode_base64(const StringView&);

String encode_base64(ReadonlyBytes);

}

using AK::decode_base64;
using AK::encode_base64;
