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

void Game::reset()
{
    m_lives = 3;
    m_pause_count = 0;
    m_cheater = false;
    reset_ball()
    reset_paddle()
    generate_bricks();
}

void Game::generate_bricks()
{
    m_bricks = {};

    Gfx::Color colors[] = {
        Gfx::Color::Red,
        Gfx::Color::Green,
        Gfx::Color::Blue,
        Gfx::Color::Yellow,
        Gfx::Color::Magenta,
        Gfx::Color::Cyan,
        Gfx::Color::LightGray,
    };

    Vector<Brick> boards[] = {
        Vector({
            Brick(0, 0, colors[3], 40, 12, 100),
            Brick(0, 4, colors[3], 40, 12, 100),
            Brick(1, 2, colors[3], 40, 12, 100),
            Brick(1, 5, colors[3], 40, 12, 100),
            Brick(2, 1, colors[3], 40, 12, 100),
            Brick(2, 3, colors[3], 40, 12, 100),
            Brick(2, 6, colors[3], 40, 12, 100),
            Brick(3, 6, colors[3], 40, 12, 100),
            Brick(4, 0, colors[3], 40, 12, 100),
            Brick(4, 6, colors[3], 40, 12, 100),
            Brick(5, 6, colors[3], 40, 12, 100),
            Brick(6, 5, colors[3], 40, 12, 100),
            Brick(7, 4, colors[3], 40, 12, 100),
        })
    };

    if (m_board != -1) {
        m_bricks = boards[m_board];
    } else {
        for (int row = 0; row < 7; ++row) {
            for (int column = 0; column < 10; ++column) {
                Brick brick(row, column, colors[row]);
                m_bricks.append(brick);
            }
        }
    }
}

}