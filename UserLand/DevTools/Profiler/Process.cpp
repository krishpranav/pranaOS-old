/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "Process.h"

namespace Profiler {

Thread* Process::find_thread(pid_t tid, EventSerialNumber serial)
{
    auto it = threads.find(tid);
    if (it == threads.end())
        return nullptr;
    for (auto& thread : it->value) {
        if (thread.start_valid < serial && (thread.end_valid == EventSerialNumber {} || thread.end_valid > serial))
            return &thread;
    }
    return nullptr;
}

void Process::handle_thread_exit(pid_t tid, EventSerialNumber serial)
{
    auto* thread = find_thread(tid, serial);
    if (!thread)
        return;
    thread->end_valid = serial;
}

HashMap<String, OwnPtr<MappedObject>> g_mapped_object_cache;


static MappedObject* get_or_create_mapped_object(const String& path)
{
    if (auto it = g_mapped_object_cache.find(path); it != g_mapped_object_cache.end())
        return it->value.ptr();

    auto file_or_error = MappedFile::map(path);
    
}

}