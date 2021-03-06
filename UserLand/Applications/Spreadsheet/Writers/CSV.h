#pragma once

// includes
#include "XSV.h"
#include <AK/Forward.h>
#include <AK/StringView.h>

namespace Writer {

template<typename ContainerType>
class CSV : public XSV<ContainerType> {
public:
    CSV(OutputStream& output, const ContainerType& data, const Vector<StringView>& headers = {}, WriterBehaviour behaviours = default_behaviours())
        : XSV<ContainerType>(output, data, { ",", "\"", WriterTraits::Repeat }, headers, behaviours)
    {
    }
};

}
