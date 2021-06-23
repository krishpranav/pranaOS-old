/*
 * Copyright (c) 2021, Krisna Pranav, nuke123-sudo
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <AK/Assertions.h>
#include <AK/Format.h>
#include <AK/GenericLexer.h>
#include <AK/StdLibExtras.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum LengthModifier {
    None,
    Default,
    Char,
    Short,
    Long,
    LongLong,
    IntMax,
    Size,
    PtrDiff,
    LongDouble
};

enum ConversionSpecifier {
    Unspecified,
    Decimal,
    Integer,
    Octal,
    Unsigned,
    Hex,
    Floating,
    String,
    UseScanList,
    Character,
    Pointer,
    OutputNumberOfBytes,
    Invalid,
};

enum class ReadKind {
    Normal,
    Octal,
    Hex,
    Infer, 
};