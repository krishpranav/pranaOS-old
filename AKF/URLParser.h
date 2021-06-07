
#pragma once

#include <AK/StringView.h>

namespace AK {

String urlencode(const StringView&, const StringView& exclude = {});
String urldecode(const StringView&);

}

using AK::urldecode;
using AK::urlencode;
