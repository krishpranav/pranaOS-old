// includes
#include <LibArchive/Zip.h>
#include <stddef.h>
#include <stdint.h>


// extern c function
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    ByteBuffer zip_data = ByteBuffer::copy(data, size);
    auto zip_file = Archive::Zip::try_create(zip_data);
    if (!zip_file.has_value())
        return 0;

    zip_file->for_each_member([]() {
        return IterationDecision::Continue;
    });

    return 0;
}