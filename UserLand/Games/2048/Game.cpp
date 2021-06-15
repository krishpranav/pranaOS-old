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
    
}