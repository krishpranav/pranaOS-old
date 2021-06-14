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

void BoardView::pick_font()
{
    String best_font_name;
    int best_font_size = -1;
    auto& font_database = Gfx::FontDatabase::the();
    font_database.for_each_font([&](const Gfx::Font& font) {
        if (font.family() != "Liza" || font.weight() != 700)
            return;
        auto size = font.glyph_height();
        if (size * 2 <= m_cell_size && size > best_font_size) {
            best_font_name = font.qualified_name();
            best_font_size = size;
        }
    });

    auto font = font_database.get_by_name(best_font_name);
    set_font(font);
}
