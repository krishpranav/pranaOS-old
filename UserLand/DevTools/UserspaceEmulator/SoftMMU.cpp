/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include "SoftMMU.h"
#include "Emulator.h"
#include "MmapRegion.h"
#include "Report.h"
#include <AK/ByteBuffer.h>
#include <AK/Memory.h>
#include <AK/QuickSort.h>

namespace UserspaceEmulator {

SoftMMU::SoftMMU(Emulator& emulator)
    : m_emulator(emulator)
{
}

}