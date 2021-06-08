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
    case Column::SampleIndex:
        return "#";
    case Column::Timestamp:
        return "Timestamp";
    case Column::ProcessID:
        return "PID";
    case Column::ThreadID:
        return "TID";
    case Column::ExecutableName:
        return "Executable";
    case Column::LostSamples:
        return "Lost Samples";
    case Column::InnermostStackFrame:
        return "Innermost Frame";
    default:
        VERIFY_NOT_REACHED();
    }
}

GUI::Variant SamplesModel::data(const GUI::ModelIndex& index, GUI::ModelRole role) const
{
    u32 event_index = m_profile.filtered_event_indices()[index.row()];
    auto& event = m_profile.events().at(event_index);

    
}

}