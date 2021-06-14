/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <AK/Format.h>
#include <AK/Platform.h>
#include <LibX86/Types.h>
#include <string.h>

namespace UserspaceEmulator {

constexpr u64 _inititalized_64 = 0x01010101'01010101LLU;
constexpr u128 _initialized_128 = u128(_inititalized_64, _inititalized_64);
constexpr u256 _initialized_256 = u256(_initialized_128, _initialized_128);


}