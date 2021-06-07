#pragma once

#include <AKF/ByteBuffer.h>
#include <AKF/String.h>
#include <AKF/StringView.h>

namespace AKF {

size_t calculate_base64_decoded_length(const StringView&);

size_t calculate_base64_encoded_length(ReadonlyBytes);

ByteBuffer decode_base64(const StringView&);

String encode_base64(ReadonlyBytes);

}

using AKF::decode_base64;
using AKF::encode_base64;
