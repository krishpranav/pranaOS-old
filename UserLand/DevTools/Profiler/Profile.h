/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */


#pragma once

// incldes
#include "DisassemblyModel.h"
#include "Process.h"
#include "Profile.h"
#include "ProfileModel.h"
#include "SamplesModel.h"
#include <AK/Bitmap.h>
#include <AK/FlyString.h>
#include <AK/JsonArray.h>
#include <AK/JsonObject.h>
#include <AK/JsonValue.h>
#include <AK/MappedFile.h>
#include <AK/NonnullRefPtrVector.h>
#include <AK/OwnPtr.h>
#include <AK/Result.h>
#include <LibELF/Image.h>
#include <LibGUI/Forward.h>
#include <LibGUI/ModelIndex.h>
