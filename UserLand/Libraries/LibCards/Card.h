/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once 

// includes
#include <AK/Array.h>
#include <AK/Format.h>
#include <LibCore/Object.h>
#include <LibGUI/Painter.h>
#include <LibGfx/Bitmap.h>
#include <LibGfx/CharacterBitmap.h>
#include <LibGfx/Rect.h>
#include <ctype.h>


namespace Cards {

class Card final : public Core::Object {
    C_OBJECT(Card)
public:
    static constexpr int width = 80;
    static constexpr int height = 100;
    static constexpr int card_count = 13;
    static constexpr int card_radius = 5;
    static constexpr Array<StringView, card_count> labels = {
        "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"
    };


}