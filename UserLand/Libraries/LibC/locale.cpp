/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <assert.h>
#include <locale.h>
#include <stdio.h>
#include <string.h>

extern "C" {

static char default_decimal_point[] = ".";
static char default_thousands_sep[] = ",";
static char default_grouping[] = "\x03\x03";

static char default_empty_string[] = "";
static char default_empty_value = 127;


}