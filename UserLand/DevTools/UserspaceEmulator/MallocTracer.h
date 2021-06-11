/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */



#pragma once

// includes
#include "MmapRegion.h"
#include "SoftMMU.h"
#include <AK/Badge.h>
#include <AK/HashMap.h>
#include <AK/OwnPtr.h>
#include <AK/Types.h>
#include <AK/Vector.h>

namespace UserspaceEmulator {

class Emulator;
class SoftCPU;

struct GraphNode {
    Vector<FlatPtr> edges_from_node {};
    
    bool is_reachable { false };
};

using MemoryGraph = HashMap<FlatPtr, GraphNode>;

struct Mallocation {
    bool contains(FlatPtr a) const 
    {
        return a >= address && a < (address + size);
    }
}

}