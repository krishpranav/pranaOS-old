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

}