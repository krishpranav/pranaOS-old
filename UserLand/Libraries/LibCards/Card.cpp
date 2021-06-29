/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include "Card.h"
#include <LibGUI/Widget.h>
#include <LibGfx/Font.h>
#include <LibGfx/FontDatabase.h>

namespace Cards {

static const NonnullRefPtr<Gfx::CharacterBitmap> s_diamond = Gfx::CharacterBitmap::create_from_ascii(
    "    #    "
    "   ###   "
    "  #####  "
    " ####### "
    "#########"
    " ####### "
    "  #####  "
    "   ###   "
    "    #    ",
    9, 9);


}