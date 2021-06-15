/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include "Game.h"
#include <AK/Array.h>
#include <AK/NumericLimits.h>
#include <AK/String.h>
#include <stdlib.h>

Game::Game(size_t grid_size, size_t target_tile, bool evil_ai)
    : m_grid_size(grid_size)
    , m_evil_ai(evil_ai)
{
    if (target_tile == 0)
        m_target_tile = 2048;
    else if ((target_tile & (target_tile -1)) != 0)
        m_target_tile = 1 << max_power_for_board(grid_size);
    else
        m_target_tile = target_tile;
    
    m_board.resize(grid_size);
    for (auto& row : m_board) {
        row.ensure_capacity(grid_size);
        for (size_t i = 0; i < grid_size; i++)
            row.append(0);
    }

    add_tile();
    add_tile();
}