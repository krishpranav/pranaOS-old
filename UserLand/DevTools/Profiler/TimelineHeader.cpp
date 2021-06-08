/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include "TimelineHeader.h"
#include "Process.h"
#include "Profile.h"
#include <AK/LexicalPath.h>
#include <LibGUI/FileIconProvider.h>
#include <LibGUI/Icon.h>
#include <LibGUI/Painter.h>
#include <LibGfx/Font.h>
#include <LibGfx/Palette.h>

namespace Profiler {


TimelineHeader::TimelineHeader(Profile& profile, Process const& process)
    : m_profile(profile)
    , m_process(process)
{
    set_frame_shape(Gfx::FrameShape::Panel);
    set_frame_shadow(Gfx::FrameShadow::Raised);
    set_fixed_size(200, 40);
    update_selection();

    m_icon = GUI::FileIconProvider::icon_for_executable(m_process.executable).bitmap_for_size(32);
    m_text = String::formatted("{} ({})", LexicalPath(m_process.executable).basename(), m_process.pid);
}

TimelineHeader::~TimelineHeader()
{
}

}