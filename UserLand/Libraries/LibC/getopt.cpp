/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <AK/StringView.h>
#include <AK/Vector.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// opt values 
int opterr = 1;
int optopt = 0;
int optind = 1;
int optreset = 0;
char* optarg = nullptr;

static size_t s_index_into_multioption_argument = 0;

static inline void report_error(const char* format, ...)
{
    if (!opterr)
        return;

    fputs("\033[31m", stderr);

    va_list ap;
    va_start(ap, format);
    vfprintf(stderr, format, ap);
    va_end(ap);

    fputs("\033[0m\n", stderr);
}

namespace {

class OptionParser {
public:
    OptionParser(int argc, char* const* argv, const StringView& short_options, const option* long_options, int* out_long_option_index = nullptr);
    int getopt();

private:
    bool lookup_short_option(char option, int& needs_value) const;
    int handle_short_option();

    const option* lookup_long_option(char* raw) const;
    int handle_long_option();

    void shift_argv();
    bool find_next_option();

    size_t m_argc { 0 };
    char* const* m_argv { nullptr };
    StringView m_short_options;
    const option* m_long_options { nullptr };
    int* m_out_long_option_index { nullptr };
    bool m_stop_on_first_non_option { false };

    size_t m_arg_index { 0 };
    size_t m_consumed_args { 0 };
};

OptionParser::OptionParser(int argc, char* const* argv, const StringView& short_options, const option* long_options, int* out_long_option_index)
    : m_argc(argc)
    , m_argv(argv)
    , m_short_options(short_options)
    , m_long_options(long_options)
    , m_out_long_option_index(out_long_option_index)
{
    m_stop_on_first_non_option = short_options.starts_with('+');

    // See if we should reset the internal state.
    if (optreset || optind == 0) {
        optreset = 0;
        optind = 1;
        s_index_into_multioption_argument = 0;
    }

    optopt = 0;
    optarg = nullptr;
}

int OptionParser::getopt()
{
    bool should_reorder_argv = !m_stop_on_first_non_option;
    int res = -1;

    bool found_an_option = find_next_option();
    StringView arg = m_argv[m_arg_index];

    if (!found_an_option) {
        res = -1;
        if (arg == "--")
            m_consumed_args = 1;
        else
            m_consumed_args = 0;
    } else {
        // Alright, so we have an option on our hands!
        bool is_long_option = arg.starts_with("--");
        if (is_long_option)
            res = handle_long_option();
        else
            res = handle_short_option();

        // If we encountered an error, return immediately.
        if (res == '?')
            return '?';
    }

    if (should_reorder_argv)
        shift_argv();
    else
        VERIFY(optind == static_cast<int>(m_arg_index));
    optind += m_consumed_args;

    return res;
}

}