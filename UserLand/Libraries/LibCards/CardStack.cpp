/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include "CardStack.h"

CardStack::CardStack()
    : m_position({ 0, 0 })
    , m_type(Invalid)
    , m_base(m_position, { Card::width, Card::height })
{
}

CardStack::CardStack(const Gfx::IntPoint& position, Type type)
    : m_position(position)
    , m_type(type)
    , m_rules(rules_for_type(type))
    , m_base(m_position, { Card::width, Card::height })
{
    VERIFY(type != Invalid);
    calculate_bounding_box();
}