/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include "TimelineContainer.h"
#include "TimelineView.h"
#include <LibGUI/Layout.h>


namespace Profiler {

TimelineContainer::TimelineContainer(GUI::Widget& header_container, TimelineView& timeline_view)
{
    set_should_hide_unnecessary_scrollbars(true);
    m_header_container = header_container;
    m_timeline_view = timeline_view;
    add_child(header_container);
    add_child(timeline_view);
    header_container.move_to_back();
    timeline_view.move_to_back();
    update_widget_sizes();
    update_widget_positions();

    m_timeline_view->on_scale_change = [this] {
        update_widget_positions();
        update_widget_sizes();
    };
}

TimelineContainer::~TimelineContainer()
{
}

}