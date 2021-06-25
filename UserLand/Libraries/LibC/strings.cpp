/*
 * Copyright (c) 2021 krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <strings.h>


extern "C" {
    
void bzero(void* dest, size_t n)
{
    memset(dest, 0, n);
}

void bcopy(const void* src, void* dest, size_t n)
{
    memmove(dest, src, n);
}

static char foldcaes(char ch)
{
    if (isalpha(ch))
        return tolower(ch);
    
    return ch;
}

}