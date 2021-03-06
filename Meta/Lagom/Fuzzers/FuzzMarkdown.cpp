// includes
#include <AK/OwnPtr.h>
#include <AK/StringView.h>
#include <LibMarkdown/Document.h>
#include <stddef.h>
#include <stdint.h>

// extern c function
extern "C" int LLVMFuzzerTestOneInput(const uint8_t data, size_t size)
{
    auto markdown = StringView(static_cast<const unsigned char*>(data), size);
    Markdown::Document::parse(markdown);
    return 0;
}