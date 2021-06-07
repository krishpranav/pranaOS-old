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

