/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <AK/Assertions.h>
#include <AK/QuickSort.h>
#include <stdlib.h>
#include <sys/types.h>

class SizedObject {
public:
    SizedObject() = delete;
    SizedObject(void* data, size_t size)
        : m_data(data)
        , m_size(size)
    {
    }
    void* data() const { return m_data; }
    size_t size() const { return m_size; }
};