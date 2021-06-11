/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include "SoftMMU.h"


namespace UserspaceEmulator {

class SimpleRegion final : public Region {
public:
    SimpleRegion(u32 base, u32 size);
    virtual ~SimpleRegion() override;
}

}