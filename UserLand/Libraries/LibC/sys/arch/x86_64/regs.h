/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <AK/Types.h>

#define RREGISTER(num)                    \
    union {                               \
        u64 r##num;                       \
        struct {                          \
            u32 _;                        \
            union {                       \
                u32 r##num##d;            \
                struct {                  \
                    u16 __;               \
                    union {               \
                        u16 r##num##w;    \
                        struct {          \
                            u8 ___;       \
                            u8 r##num##b; \
                        };                \
                    };                    \
                };                        \
            };                            \
        };                                \
    }

#define GPREGISTER(letter)                \
    union {                               \
        u64 r##letter##x;                 \
        struct                            \
        {                                 \
            u32 _;                        \
            union {                       \
                u32 e##letter##x;         \
                struct                    \
                {                         \
                    u16 __;               \
                    union {               \
                        u16 letter##x;    \
                        struct {          \
                            u8 letter##h; \
                            u8 letter##l; \
                        };                \
                    };                    \
                };                        \
            };                            \
        };                                \
    }

#define SPREGISTER(name)                \
    union {                             \
        u64 r##name;                    \
        struct                          \
        {                               \
            u32 _;                      \
            union {                     \
                u32 e##name;            \
                struct                  \
                {                       \
                    u16 __;             \
                    union {             \
                        u16 name;       \
                        struct {        \
                            u8 ___;     \
                            u8 name##l; \
                        };              \
                    };                  \
                };                      \
            };                          \
        };                              \
    }

struct [[gnu::packed]] PtraceRegisters {
    GPREGISTER(a);
    GPREGISTER(b);
    GPREGISTER(c);
    GPREGISTER(d);

    SPREGISTER(sp);
    SPREGISTER(bp);
    SPREGISTER(si);
    SPREGISTER(di);
    SPREGISTER(ip); // technically there is no ipl, but what ever

    RREGISTER(8);
    RREGISTER(9);
    RREGISTER(10);
    RREGISTER(11);
    RREGISTER(12);
    RREGISTER(13);
    RREGISTER(14);
    RREGISTER(15);
    // flags
    union {
        u64 rflags;
        struct {
            u32 _;
            union {
                u32 eflags;
                struct {
                    u16 __;
                    u16 flags;
                };
            };
        };
    };

    // These may not be used, unless we go back into compatibility mode
    u32 cs;
    u32 ss;
    u32 ds;
    u32 es;
    u32 fs;
    u32 gs;

    // FIXME: Add FPU registers and Flags
    // FIXME: Add Ymm Xmm etc.
};
