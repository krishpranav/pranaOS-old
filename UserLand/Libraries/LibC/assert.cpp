/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <AK/Format.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/internals.h>
#include <syscall.h>
#include <unistd.h>

extern "C" {


extern bool __stdio_is_initialized;
#ifndef DEBUG 
void __assertion_failed(const char* msg)
{
    
}

}