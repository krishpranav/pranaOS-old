/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include "EventSerialNumber.h"
#include <AK/HashMap.h>
#include <AK/MappedFile.h>
#include <AK/OwnPtr.h>
#include <AK/Vector.h>
#include <LibELF/Image.h>

namespace Profiler {

    struct MappedObject {
        NonnullRefPtr<MappedFile> file;
        ELF::Image elf;
    }; 

    extern HashMap<String, OwnPtr<MappedObject>> g_mapped_object_cache;



}