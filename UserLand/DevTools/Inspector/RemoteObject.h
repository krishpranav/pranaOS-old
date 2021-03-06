#pragma once

// includes
#include <AK/JsonObject.h>
#include <AK/NonnullOwnPtrVector.h>
#include <AK/String.h>
#include <AK/Vector.h>

namespace Inspector {

class RemoteObjectPropertyModel;

class RemoteObject {
public:
    RemoteObject();

    RemoteObjectPropertyModel& property_model();

    RemoteObject* parent { nullptr };
    NonnullOwnPtrVector<RemoteObject> children;

    FlatPtr address { 0 };
    FlatPtr parent_address { 0 };
    String class_name;
    String name;

    JsonObject json;

    NonnullRefPtr<RemoteObjectPropertyModel> m_property_model;
};

}
