/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include "ProfileModel.h"
#include "Profile.h"
#include <AK/StringBuilder.h>
#include <LibGUI/FileIconProvider.h>
#include <ctype.h>
#include <stdio.h>

namespace  Profiler {

    ProfileModel::ProfileModel(Profile& profile)
            : m_profile(profile)
    {
        m_user_frame_icon.set_bitmap_for_size(16, Gfx::Bitmap::load_from_file("/res/icons/16x16/inspector-object.png"));
        m_kernel_frame_icon.set_bitmap_for_size(16, Gfx::Bitmap::load_from_file("/res/icons/16x16/inspector-object-red.png"));
    }

}