/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <AK/String.h>
#include <AK/StringView.h>
#include <string.h>
#include <sys/types.h>

namespace Archive {

enum class TarFileType : char {
    NormalFile = '0',
    AlternateNormalFile = '\0',
    HardLink = '1',
    SymLink = '2',
    CharacterSpecialFile = '3',
    BlockSpecialFile = '4',
    Directory = '5',
    FIFO = '6',
    ContiguousFile = '7',
    GlobalExtendedHeader = 'g',
    ExtendedHeader = 'x'
};


}