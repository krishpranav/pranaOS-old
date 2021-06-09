/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <AK/GenericLexer.h>
#include <AK/HashTable.h>
#include <AK/OwnPtr.h>
#include <AK/SourceGenerator.h>
#include <AK/String.h>
#include <AK/StringBuilder.h>
#include <AK/Types.h>
#include <LibCore/ArgsParser.h>
#include <LibCore/File.h>
#include <ctype.h>
#include <string.h>

struct Range {
    int begin;
    int end;
};

struct StateTransition {
    Optional<String> new_state;
    Optional<String> action;
};