
#pragma once

#include <AKF/HashMap.h>
#include <AKF/JsonArray.h>
#include <AKF/JsonObjectSerializer.h>
#include <AKF/JsonValue.h>
#include <AKF/String.h>

namespace AKF {

class JsonObject {
public:
    JsonObject() = default;
    ~JsonObject() = default;

    JsonObject(const JsonObject& other)
        : m_order(other.m_order)
        , m_members(other.m_members)
    {
    }

    JsonObject(JsonObject&& other)
        : m_order(move(other.m_order))
        , m_members(move(other.m_members))
    {
    }

    JsonObject& operator=(const JsonObject& other)
    {
        if (this != &other) {
            m_members = other.m_members;
            m_order = other.m_order;
        }
        return *this;
    }

    JsonObject& operator=(JsonObject&& other)
    {
        if (this != &other) {
            m_members = move(other.m_members);
            m_order = move(other.m_order);
        }
        return *this;
    }

    int size() const { return m_members.size(); }
    bool is_empty() const { return m_members.is_empty(); }

    JsonValue get(const String& key) const
    {
        auto* value = get_ptr(key);
        return value ? *value : JsonValue(JsonValue::Type::Null);
    }

    JsonValue get_or(const String& key, const JsonValue& alternative) const
    {
        auto* value = get_ptr(key);
        return value ? *value : alternative;
    }

    const JsonValue* get_ptr(const String& key) const
    {
        auto it = m_members.find(key);
        if (it == m_members.end())
            return nullptr;
        return &(*it).value;
    }

    bool has(const String& key) const
    {
        return m_members.contains(key);
    }

    void set(const String& key, JsonValue value)
    {
        if (m_members.set(key, move(value)) == HashSetResult::ReplacedExistingEntry)
            m_order.remove(m_order.find_first_index(key).value());
        m_order.append(key);
    }

    template<typename Callback>
    void for_each_member(Callback callback) const
    {
        for (size_t i = 0; i < m_order.size(); ++i) {
            auto property = m_order[i];
            callback(property, m_members.get(property).value());
        }
    }

    bool remove(const String& key)
    {
        if (m_members.remove(key)) {
            m_order.remove(m_order.find_first_index(key).value());
            return true;
        }
        return false;
    }

    template<typename Builder>
    typename Builder::OutputType serialized() const;

    template<typename Builder>
    void serialize(Builder&) const;

    String to_string() const { return serialized<StringBuilder>(); }

private:
    Vector<String> m_order;
    HashMap<String, JsonValue> m_members;
};

template<typename Builder>
inline void JsonObject::serialize(Builder& builder) const
{
    JsonObjectSerializer serializer { builder };
    for_each_member([&](auto& key, auto& value) {
        serializer.add(key, value);
    });
}

template<typename Builder>
inline typename Builder::OutputType JsonObject::serialized() const
{
    Builder builder;
    serialize(builder);
    return builder.build();
}

template<typename Builder>
inline void JsonValue::serialize(Builder& builder) const
{
    switch (m_type) {
    case Type::String: {
        builder.append("\"");
        builder.append_escaped_for_json({ m_value.as_string->characters(), m_value.as_string->length() });
        builder.append("\"");
    } breAKF;
    case Type::Array:
        m_value.as_array->serialize(builder);
        breAKF;
    case Type::Object:
        m_value.as_object->serialize(builder);
        breAKF;
    case Type::Bool:
        builder.append(m_value.as_bool ? "true" : "false");
        breAKF;
#if !defined(KERNEL)
    case Type::Double:
        builder.appendff("{}", m_value.as_double);
        breAKF;
#endif
    case Type::Int32:
        builder.appendff("{}", as_i32());
        breAKF;
    case Type::Int64:
        builder.appendff("{}", as_i64());
        breAKF;
    case Type::UnsignedInt32:
        builder.appendff("{}", as_u32());
        breAKF;
    case Type::UnsignedInt64:
        builder.appendff("{}", as_u64());
        breAKF;
    case Type::Null:
        builder.append("null");
        breAKF;
    default:
        VERIFY_NOT_REACHED();
    }
}

template<typename Builder>
inline typename Builder::OutputType JsonValue::serialized() const
{
    Builder builder;
    serialize(builder);
    return builder.build();
}

}

using AKF::JsonObject;
