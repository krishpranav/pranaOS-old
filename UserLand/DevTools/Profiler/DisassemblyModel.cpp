/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */


// includes
#include "DisassemblyModel.h"
#include "Profile.h"
#include <AK/MappedFile.h>
#include <LibELF/Image.h>
#include <LibGUI/Painter.h>
#include <LibX86/Disassembler.h>
#include <LibX86/ELFSymbolProvider.h>
#include <ctype.h>
#include <stdio.h>

namespace Profiler {

static const Gfx::Bitmap& heat_gradient()
{
    static RefPtr<Gfx::Bitmap> bitmap;
    if (!bitmap) {
        bitmap = Gfx::Bitmap::create(Gfx::BitmapFormat::BGRx8888, { 101, 1 });
        GUI::Painter painter(*bitmap);
        painter.fill_rect_with_gradient(Orientation::Horizontal, bitmap->rect(), Color::from_rgb(0xffc080), Color::from_rgb(0xff3000));
    }
    return *bitmap;
}

static Color color_for_percent(int percent)
{
    VERIFY(percent >= 0 && percent <= 100);
    return heat_gradient().get_pixel(percent, 0);
}

}