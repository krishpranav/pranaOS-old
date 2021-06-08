/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include "TimelineView.h"
#include "Profile.h"
#include "TimelineTrack.h"
#include <LibGUI/BoxLayout.h>

namespace Profiler {

TimelineView::TimelineView(Profile& profile)
    : m_profile(profile)
{
    set_layout<GUI::VerticalBoxLayout>();
    set_shrink_to_fit(true);
}

TimelineView::~TimelineView()
{
}

u64 TimelineView::timestamp_at_x(int x) const
{
    float column_width = (float)width() / (float)m_profile.length_in_ms();
    float ms_into_profile = (float)x / column_width;
    return m_profile.first_timestamp() + (u64)ms_into_profile;
}

void TimelineView::mousedown_event(GUI::MouseEvent& event)
{
    if (event.button() != GUI::MouseButton::Left)
        return;

    set_selecting(true);
    set_select_start_time(timestamp_at_x(event.x()));
    set_select_end_time(select_start_time());
    m_profile.set_timestamp_filter_range(select_start_time(), select_end_time());
    update();
}

}