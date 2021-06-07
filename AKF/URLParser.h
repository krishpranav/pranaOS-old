
#pragma once

#include <AKF/StringView.h>

namespace AKF {

String urlencode(const StringView&, const StringView& exclude = {});
String urldecode(const StringView&);

}

using AKF::urldecode;
using AKF::urlencode;
