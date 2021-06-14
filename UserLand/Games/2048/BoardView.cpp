/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include "BoardView.h"
#include <LibGUI/Painter.h>
#include <LibGfx/Font.h>
#include <LibGfx/FontDatabase.h>
#include <LibGfx/Palette.h>

BoardView::BoardView(const Game::Board* board)
    : m_board(board)
{
}

BoardView::~BoardView()
{
}

void BoardView::set_board(const Game::Board* board)
{
    if (m_board == board)
        return;

    if (!board) {
        m_board = nullptr;
        return;
    }

    bool must_resize = !m_board || m_board->size() != board->size();

    m_board = board;

    if (must_resize)
        resize();

    update();
}