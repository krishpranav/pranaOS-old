#pragma once

// includes
#include <AK/String.h>
#include <AK/Vector.h>
#include <Kernel/KBuffer.h>

namespace Kernel {

typedef void* (*ModuleInitPtr)();
typedef void* (*ModuleFiniPtr)();

struct Module {
    String name;
    Vector<KBuffer> sections;

    ModuleInitPtr module_init { nullptr };
    ModuleFiniPtr module_fini { nullptr };
};

}