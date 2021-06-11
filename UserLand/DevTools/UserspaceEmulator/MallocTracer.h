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

    FlatPtr address { 0 };
    size_t size { 0 };
    bool used { false };
    bool freed { false };

    Vector<FlatPtr> malloc_backtrace;
    Vector<FlatPtr> free_backtrace;
};

class MallocRegionMetadata {
public:
    MmapRegion& region;
    FlatPtr address { 0 };
    size_t chunk_size { 0 };

    Optional<size_t> chunk_index_for_address(FlatPtr) const;
    Mallocation* mallocation_for_address(FlatPtr) const;

    Vector<Mallocation> mallocation;
};

}