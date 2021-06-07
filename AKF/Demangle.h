
#pragma once

#include <AK/String.h>
#include <AK/StringView.h>

#ifndef BUILDING_pranaos_TOOLCHAIN
#    include <cxxabi.h>
#endif

namespace AK {

inline String demangle(const StringView& name)
{
#ifdef BUILDING_pranaos_TOOLCHAIN
    return name;
#else
    int status = 0;
    auto* demangled_name = abi::__cxa_demangle(name.to_string().characters(), nullptr, nullptr, &status);
    auto string = String(status == 0 ? demangled_name : name);
    if (status == 0)
        kfree(demangled_name);
    return string;
#endif
}

}

using AK::demangle;
