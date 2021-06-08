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


}