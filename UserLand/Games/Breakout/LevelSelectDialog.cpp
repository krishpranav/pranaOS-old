/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */


// includes
#include "LevelSelectDialog.h"
#include <LibGUI/BoxLayout.h>
#include <LibGUI/Button.h>
#include <LibGUI/Label.h>
#include <LibGUI/ListView.h>

namespace Breakout {


LevelSelectDialog::LevelSelectDialog(Window* parent_window)
    : Dialog(parent_window)
{
    set_rect(0, 0, 300, 250);
    set_title("Level Select");
    build();
}

LevelSelectDialog::~LevelSelectDialog()
{
}

}