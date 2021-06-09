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

static OwnPtr<StateMachine>
parse_state_machine(StringView input)
{
    auto state_machine = make<StateMachine>();
    GenericLexer lexer(input);

    auto consume_whitespace = [&] {
        bool consumed = true;
        while (consumed) {
            consumed = lexer.consume_while(isspace).length() > 0;
            if (lexer.consume_specific("//")) {
                lexer.consume_line();
                consumed = true;
            }
        }
    };

    auto consume_identifier = [&] {
        consume_whitespace();
        return lexer.consume_while([](char c) { return isalnum(c) || c == '_'; });
    };

    auto get_hex_value = [&](char c) {
        if (isdigit(c))
            return c - '0';
        else
            return c - 'a' + 10;
    };

    auto consume_number = [&] {
        int num = 0;
        consume_whitespace();
        if (lexer.consume_specific("0x")) {
            auto hex_digits = lexer.consume_while([](char c) {
                if (isdigit(c)) return true;
            else {
                c = tolower(c);
                return (c >= 'a' && c <= 'f');
              } });
            for (auto c : hex_digits)
                num = 16 * num + get_hex_value(c);
        } else {
            lexer.consume_specific('\'');
            if (lexer.next_is('\\'))
                num = (int)lexer.consume_escaped_character('\\');
            else
                num = lexer.consume_until('\'').to_int().value();
            lexer.consume_specific('\'');
        }
        return num;
    };

    auto consume_condition = [&] {
        Range condition;
        consume_whitespace();
        if (lexer.consume_specific('[')) {
            consume_whitespace();
            condition.begin = consume_number();
            consume_whitespace();
            lexer.consume_specific("..");
            consume_whitespace();
            condition.end = consume_number();
            consume_whitespace();
            lexer.consume_specific(']');
        } else {
            auto num = consume_number();
            condition.begin = num;
            condition.end = num;
        }
        return condition;
    };

    auto consume_action = [&]() {
        StateTransition action;
        consume_whitespace();
        lexer.consume_specific("=>");
        consume_whitespace();
        lexer.consume_specific('(');
        consume_whitespace();
        if (!lexer.consume_specific("_"))
            action.new_state = consume_identifier();
        consume_whitespace();
        lexer.consume_specific(',');
        consume_whitespace();
        if (!lexer.consume_specific("_"))
            action.action = consume_identifier();
        consume_whitespace();
        lexer.consume_specific(')');
        return action;
    };

    auto consume_state_description
        = [&] {
              State state;
              consume_whitespace();
              state.name = consume_identifier();
              consume_whitespace();
              consume_whitespace();
              lexer.consume_specific('{');
              for (;;) {
                  consume_whitespace();
                  if (lexer.consume_specific('}')) {
                      break;
                  }
                  if (lexer.consume_specific("@entry")) {
                      consume_whitespace();
                      state.entry_action = consume_identifier();
                  } else if (lexer.consume_specific("@exit")) {
                      consume_whitespace();
                      state.exit_action = consume_identifier();
                  } else if (lexer.next_is('@')) {
                      auto directive = consume_identifier().to_string();
                      fprintf(stderr, "Unimplemented @ directive %s\n", directive.characters());
                      exit(1);
                  } else {
                      MatchedAction matched_action;
                      matched_action.range = consume_condition();
                      matched_action.action = consume_action();
                      state.actions.append(matched_action);
                  }
              }
              return state;
          };

    while (!lexer.is_eof()) {
        consume_whitespace();
        if (lexer.is_eof())
            break;
        if (lexer.consume_specific("@namespace")) {
            consume_whitespace();
            state_machine->namespaces = lexer.consume_while([](char c) { return isalpha(c) || c == ':'; });
        } else if (lexer.consume_specific("@begin")) {
            consume_whitespace();
            state_machine->initial_state = consume_identifier();
        } else if (lexer.consume_specific("@name")) {
            consume_whitespace();
            state_machine->name = consume_identifier();
        } else if (lexer.next_is("@anywhere")) {
            lexer.consume_specific('@');
            state_machine->anywhere = consume_state_description();
        } else if (lexer.consume_specific('@')) {
            auto directive = consume_identifier().to_string();
            fprintf(stderr, "Unimplemented @ directive %s\n", directive.characters());
            exit(1);
        } else {
            auto description = consume_state_description();
            state_machine->states.append(description);
        }
    }

    if (state_machine->initial_state.is_empty()) {
        fprintf(stderr, "Missing @begin directive\n");
        exit(1);
    } else if (state_machine->name.is_empty()) {
        fprintf(stderr, "Missing @name directive\n");
        exit(1);
    }

    if (state_machine->anywhere.has_value()) {
        state_machine->anywhere.value().name = "_Anywhere";
    }
    return state_machine;
}

void output_header(const StateMachine&, SourceGenerator&);


int main(int argc, char** argv)
{
    Core::ArgsParser args_parser;
    const char* path = nullptr;
    args_parser.add_positional_argument(path, "Path to parser description", "input", Core::ArgsParser::Required::Yes);
    args_parser.parse(argc, argv);

    auto file_or_error = Core::File::open(path, Core::OpenMode::ReadOnly);
    if (file_or_error.is_error()) {
        fprintf(stderr, "Cannot open %s\n", path);
    }

    auto content = file_or_error.value()->read_all();
    auto state_machine = parse_state_machine(content);
}