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

struct MatchedAction {
    Range range;
    StateTransition action;
};

struct State {
    String name;
    Vector<MatchedAction> actions;
    Optional<String> entry_action;
    Optional<String> exit_action;
};

struct StateMachine {
    String name;
    String initial_state;
    Vector<State> states;
    Optional<State> anywhere;
    Optional<String> namespaces;
};