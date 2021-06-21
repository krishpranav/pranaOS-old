/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// incldes
#include <stdint.h>
#include <sys/cdefs.h>


struct __x87_floating_point_environment {
    uint16_t __control_word;
    uint16_t __reserved1;
    uint16_t __status_word;
    uint16_t __reserved2;
    uint16_t __tag_word;
    uint16_t __reserved3;
    uint32_t __fpu_ip_offset;
    uint16_t __fpu_ip_selector;
    uint16_t __opcode : 11;
    uint16_t __reserved4 : 5;
    uint32_t __fpu_data_offset;
    uint16_t __fpu_data_selector;
    uint16_t __reserved5;
};