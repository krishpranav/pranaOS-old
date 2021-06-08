/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


#pragma once

#include <LibGUI/Model.h>

namespace Profiler {

class Profile;

class SamplesModel final : public GUI::Model {
public:
    static NonnullRefPtr<SamplesModel> create(Profile& profile)
    {
        return adopt_ref(*new SamplesModel(profile));
    }

    enum Column {
        SampleIndex,
        Timestamp,
        ProcessID,
        ThreadID,
        ExecutableName,
        LostSamples,
        InnermostStackFrame,
        __Count
    };


}

}