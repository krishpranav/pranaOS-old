/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once 

// includes
#include "Game.h"
#include <LibGUI/Frame.h>

class BoardView final : public GUI::Frame {
    C_OBJECT(BoardView);

public:
    virtual ~BoardView() override;
    void set_board(const Game::Board* board);

    Function<void(Game::Direction)> on_move;
}