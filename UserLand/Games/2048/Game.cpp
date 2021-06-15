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

void Game::add_random_tile()
{
    int row;
    int column;
    do {
        row = rand() % m_grid_size;
        column = rand() % m_grid_size;
    } while (m_board[row][column] != 0);

    size_t value = rand() < RAND_MAX * 0.9 ? 2 : 4;
    m_board[row][column] = value;
}

static Game::Board transpose(const Game::Board& board)
{
    Vector<Vector<u32>> new_board;
    auto result_row_count = board[0].size();
    auto result_column_count = board.size();

    new_board.resize(result_row_count);

    for (size_t i = 0; i < board.size(); ++i) {
        auto& row = new_board[i];
        row.clear_with_capacity();
        row.ensure_capacity(result_column_count);
        for (auto& entry : board) {
            row.append(entry[i]);
        }
    }

    return new_board;
}

static Game::Board reverse(const Game::Board& board)
{
    auto new_board = board;
    for (auto& row : new_board) {
        for (size_t i = 0; i < row.size() / 2; ++i)
            swap(row[i], row[row.size() - i - 1]);
    }

    return new_board;
}
