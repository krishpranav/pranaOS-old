
#pragma once

#include <AKF/Types.h>

namespace AKF {

class Bitmap;
class ByteBuffer;
class IPv4Address;
class JsonArray;
class JsonObject;
class JsonValue;
class StackInfo;
class String;
class StringBuilder;
class StringImpl;
class StringView;
class Time;
class URL;
class FlyString;
class Utf32View;
class Utf8View;
class InputStream;
class InputMemoryStream;
class DuplexMemoryStream;
class OutputStream;
class InputBitStream;
class OutputBitStream;
class OutputMemoryStream;

template<size_t Capacity>
class CircularDuplexStream;

template<typename T>
class Span;

template<typename T, size_t Size>
struct Array;

template<typename Container, typename ValueType>
class SimpleIterator;

using ReadonlyBytes = Span<const u8>;
using Bytes = Span<u8>;

template<typename T, AKF::MemoryOrder DefaultMemoryOrder>
class Atomic;

template<typename T>
class SinglyLinkedList;

template<typename T>
class DoublyLinkedList;

template<typename T>
class InlineLinkedList;

template<typename T, size_t capacity>
class CircularQueue;

template<typename T>
struct Traits;

template<typename T, typename = Traits<T>>
class HashTable;

template<typename K, typename V, typename = Traits<K>>
class HashMap;

template<typename T>
class Badge;

template<typename>
class Function;

template<typename Out, typename... In>
class Function<Out(In...)>;

template<typename T>
class NonnullRefPtr;

template<typename T>
class NonnullOwnPtr;

template<typename T, size_t inline_capacity = 0>
class NonnullRefPtrVector;

template<typename T, size_t inline_capacity = 0>
class NonnullOwnPtrVector;

template<typename T>
class Optional;

template<typename T>
struct RefPtrTraits;

template<typename T, typename PtrTraits = RefPtrTraits<T>>
class RefPtr;

template<typename T>
class OwnPtr;

template<typename T>
class WeAKFPtr;

template<typename T, size_t inline_capacity = 0>
class Vector;

}

using AKF::Array;
using AKF::Atomic;
using AKF::Badge;
using AKF::Bitmap;
using AKF::ByteBuffer;
using AKF::Bytes;
using AKF::CircularDuplexStream;
using AKF::CircularQueue;
using AKF::DoublyLinkedList;
using AKF::DuplexMemoryStream;
using AKF::FlyString;
using AKF::Function;
using AKF::HashMap;
using AKF::HashTable;
using AKF::InlineLinkedList;
using AKF::InputBitStream;
using AKF::InputMemoryStream;
using AKF::InputStream;
using AKF::IPv4Address;
using AKF::JsonArray;
using AKF::JsonObject;
using AKF::JsonValue;
using AKF::NonnullOwnPtr;
using AKF::NonnullOwnPtrVector;
using AKF::NonnullRefPtr;
using AKF::NonnullRefPtrVector;
using AKF::Optional;
using AKF::OutputBitStream;
using AKF::OutputMemoryStream;
using AKF::OutputStream;
using AKF::OwnPtr;
using AKF::ReadonlyBytes;
using AKF::RefPtr;
using AKF::SinglyLinkedList;
using AKF::Span;
using AKF::StackInfo;
using AKF::String;
using AKF::StringBuilder;
using AKF::StringImpl;
using AKF::StringView;
using AKF::Time;
using AKF::Traits;
using AKF::URL;
using AKF::Utf32View;
using AKF::Utf8View;
using AKF::Vector;
