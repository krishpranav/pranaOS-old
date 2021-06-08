/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


#pragma once

#include <LibGUI/AbstractScrollableWidget.h>

namespace Profiler {


class TimelineView;

class TimelineContainer : public GUI::AbstractScrollableWidget {
    C_OBJECT(TimelineContainer);

public:
    virtual ~TimelineContainer();



}

}