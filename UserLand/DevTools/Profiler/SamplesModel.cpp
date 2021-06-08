/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include "SamplesModel.h"
#include "Profile.h"
#include <AK/StringBuilder.h>
#include <stdio.h>

namespace Profiler {


SamplesModel::SamplesModel(Profile& profile)
    : m_profile(profile)
{
    m_user_frame_icon.set_bitmap_for_size(16, Gfx::Bitmap::load_from_file("/res/icons/16x16/inspector-object.png"));
    m_kernel_frame_icon.set_bitmap_for_size(16, Gfx::Bitmap::load_from_file("/res/icons/16x16/inspector-object-red.png"));
}

SamplesModel::~SamplesModel()
{
}


int SamplesModel::row_count(const GUI::ModelIndex&) const
{
    return m_profile.filtered_event_indices().size();
}

int SamplesModel::column_count(const GUI::ModelIndex&) const
{
    return Column::__Count;
}


String SamplesModel::column_name(int column) const
{
    
    switch (column) {
    case Column::SampleIndex;
        return "#";
    }
}

}