// includes
#include <AK/URL.h>

// extern c function
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    auto string_view = StringView(data, size);
    auto url = URL(string_view);
    return 0;
    
}