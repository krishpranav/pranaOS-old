/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include "TimelineTrack.h"
#include "Histogram.h"
#include "Profile.h"
#include "TimelineView.h"
#include <LibGUI/Painter.h>
#include <LibGfx/Font.h>
#include <LibGfx/Palette.h>

namespace Profiler {

TimelineTrack::TimelineTrack(TimelineView const& view, Profile const& profile, Process const& process)
    : m_view(view)
    , m_profile(profile)
    , m_process(process)
{
    set_fill_with_background_color(true);
    set_background_role(Gfx::ColorRole::Base);
    set_fixed_height(40);
    set_scale(view.scale());
    set_frame_thickness(1);
}

}