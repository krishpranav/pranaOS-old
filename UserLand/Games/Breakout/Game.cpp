/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include "Game.h"
#include "LevelSelectDialog.h"
#include <AK/Random.h>
#include <LibGUI/Application.h>
#include <LibGUI/MessageBox.h>
#include <LibGUI/Painter.h>
#include <LibGfx/Font.h>
#include <LibGfx/StandardCursor.h>
#include <unistd.h>

namespace Breakout {

Game::Game()
{
    set_override_cursor(Gfx::StandardCursor::Hidden);
    auto level_dialog = LevelSelectDialog::show(m_board, window());
    if (level_dialog != GUI::Dialog::ExecOK)
        m_board = -1;
    set_paused(false);
    start_timer(16);
    reset();
}


Game::~Game()
{
}

void Game::reset_paddle()
{
    m_paddle.moving_left = false;
    m_paddle.moving_right = false;
    m_paddle.rect = { game_width / 2 - 40, game_height - 20, 80, 16 };
}


}