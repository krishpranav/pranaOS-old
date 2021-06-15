/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <AK/Types.h>
#include <LibGUI/Dialog.h>

class GameSizeDialog : public GUI::Dialog {
    C_OBJECT(GameSizeDialog)
public:
    size_t board_size() const { return m_board_size; }
    u32 target_tile() const { return 1u << m_target_tile_power; }
    bool evil_ai() const { return m_evil_ai; }
    bool temporary() const { return m_temporary; }
}