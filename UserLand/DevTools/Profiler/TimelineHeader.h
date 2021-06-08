/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <LibGUI/Frame.h>

namespace Profiler {

class Profile;
class Process;

class TimelineHeader final : public GUI::Frame {
    C_OBJECT(TimelineHeader);

}

}