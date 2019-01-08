// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#ifdef __cplusplus

#define ARDUINOJSON_VERSION "6.7.0-beta"
#define ARDUINOJSON_VERSION_MAJOR 6
#define ARDUINOJSON_VERSION_MINOR 7
#define ARDUINOJSON_VERSION_REVISION 0
#if defined(_MSC_VER)
#define ARDUINOJSON_HAS_INT64 1
#else
#define ARDUINOJSON_HAS_INT64 0
#endif
#if __cplusplus >= 201103L
#define ARDUINOJSON_HAS_LONG_LONG 1
#else
#define ARDUINOJSON_HAS_LONG_LONG 0
#endif
#ifndef ARDUINOJSON_EMBEDDED_MODE
#if defined(ARDUINO) || defined(__IAR_SYSTEMS_ICC__) || defined(__XC) || \
    defined(__ARMCC_VERSION)
#define ARDUINOJSON_EMBEDDED_MODE 1
#else
#define ARDUINOJSON_EMBEDDED_MODE 0
#endif
#endif
#if ARDUINOJSON_EMBEDDED_MODE
#ifndef ARDUINOJSON_USE_DOUBLE
#define ARDUINOJSON_USE_DOUBLE 0
#endif
#ifndef ARDUINOJSON_USE_LONG_LONG
#define ARDUINOJSON_USE_LONG_LONG 0
#endif
#ifndef ARDUINOJSON_ENABLE_STD_STRING
#define ARDUINOJSON_ENABLE_STD_STRING 0
#endif
#ifndef ARDUINOJSON_ENABLE_STD_STREAM
#define ARDUINOJSON_ENABLE_STD_STREAM 0
#endif
#ifndef ARDUINOJSON_DEFAULT_NESTING_LIMIT
#define ARDUINOJSON_DEFAULT_NESTING_LIMIT 10
#endif
#ifndef ARDUINOJSON_DEFAULT_POOL_SIZE
#define ARDUINOJSON_DEFAULT_POOL_SIZE 1024
#endif
#else  // ARDUINOJSON_EMBEDDED_MODE
#ifndef ARDUINOJSON_USE_DOUBLE
#define ARDUINOJSON_USE_DOUBLE 1
#endif
#ifndef ARDUINOJSON_USE_LONG_LONG
#if ARDUINOJSON_HAS_LONG_LONG || ARDUINOJSON_HAS_INT64
#define ARDUINOJSON_USE_LONG_LONG 1
#else
#define ARDUINOJSON_USE_LONG_LONG 0
#endif
#endif
#ifndef ARDUINOJSON_ENABLE_STD_STRING
#define ARDUINOJSON_ENABLE_STD_STRING 1
#endif
#ifndef ARDUINOJSON_ENABLE_STD_STREAM
#define ARDUINOJSON_ENABLE_STD_STREAM 1
#endif
#ifndef ARDUINOJSON_DEFAULT_NESTING_LIMIT
#define ARDUINOJSON_DEFAULT_NESTING_LIMIT 50
#endif
#ifndef ARDUINOJSON_DEFAULT_POOL_SIZE
#define ARDUINOJSON_DEFAULT_POOL_SIZE 16384
#endif
#endif  // ARDUINOJSON_EMBEDDED_MODE
#ifdef ARDUINO
#ifndef ARDUINOJSON_ENABLE_ARDUINO_STRING
#define ARDUINOJSON_ENABLE_ARDUINO_STRING 1
#endif
#ifndef ARDUINOJSON_ENABLE_ARDUINO_STREAM
#define ARDUINOJSON_ENABLE_ARDUINO_STREAM 1
#endif
#else  // ARDUINO
#ifndef ARDUINOJSON_ENABLE_ARDUINO_STRING
#define ARDUINOJSON_ENABLE_ARDUINO_STRING 0
#endif
#ifndef ARDUINOJSON_ENABLE_ARDUINO_STREAM
#define ARDUINOJSON_ENABLE_ARDUINO_STREAM 0
#endif
#endif  // ARDUINO
#ifndef ARDUINOJSON_ENABLE_PROGMEM
#ifdef PROGMEM
#define ARDUINOJSON_ENABLE_PROGMEM 1
#else
#define ARDUINOJSON_ENABLE_PROGMEM 0
#endif
#endif
#ifndef ARDUINOJSON_POSITIVE_EXPONENTIATION_THRESHOLD
#define ARDUINOJSON_POSITIVE_EXPONENTIATION_THRESHOLD 1e7
#endif
#ifndef ARDUINOJSON_NEGATIVE_EXPONENTIATION_THRESHOLD
#define ARDUINOJSON_NEGATIVE_EXPONENTIATION_THRESHOLD 1e-5
#endif
#ifndef ARDUINOJSON_LITTLE_ENDIAN
#if defined(_MSC_VER) ||                                                      \
    (defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__) || \
    (defined(__LITTLE_ENDIAN__))
#define ARDUINOJSON_LITTLE_ENDIAN 1
#else
#define ARDUINOJSON_LITTLE_ENDIAN 0
#endif
#endif
#define ARDUINOJSON_DO_CONCAT(A, B) A##B
#define ARDUINOJSON_CONCAT2(A, B) ARDUINOJSON_DO_CONCAT(A, B)
#define ARDUINOJSON_CONCAT4(A, B, C, D) \
  ARDUINOJSON_CONCAT2(ARDUINOJSON_CONCAT2(A, B), ARDUINOJSON_CONCAT2(C, D))
#define ARDUINOJSON_CONCAT8(A, B, C, D, E, F, G, H)    \
  ARDUINOJSON_CONCAT2(ARDUINOJSON_CONCAT4(A, B, C, D), \
                      ARDUINOJSON_CONCAT4(E, F, G, H))
#define ARDUINOJSON_NAMESPACE                                                  \
  ARDUINOJSON_CONCAT8(ArduinoJson, ARDUINOJSON_VERSION_MAJOR,                  \
                      ARDUINOJSON_VERSION_MINOR, ARDUINOJSON_VERSION_REVISION, \
                      _, ARDUINOJSON_USE_LONG_LONG, _, ARDUINOJSON_USE_DOUBLE)
#ifdef ARDUINOJSON_DEBUG
#include <assert.h>
#define ARDUINOJSON_ASSERT(X) assert(X)
#else
#define ARDUINOJSON_ASSERT(X) ((void)0)
#endif
#include <stddef.h>  // for size_t
namespace ARDUINOJSON_NAMESPACE {
template <size_t X, size_t Y, bool MaxIsX = (X > Y)>
struct Max {};
template <size_t X, size_t Y>
struct Max<X, Y, true> {
  static const size_t value = X;
};
template <size_t X, size_t Y>
struct Max<X, Y, false> {
  static const size_t value = Y;
};
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
template <bool Condition, class TrueType, class FalseType>
struct conditional {
  typedef TrueType type;
};
template <class TrueType, class FalseType>
struct conditional<false, TrueType, FalseType> {
  typedef FalseType type;
};
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
template <bool Condition, typename T = void>
struct enable_if {};
template <typename T>
struct enable_if<true, T> {
  typedef T type;
};
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
template <typename T, T v>
struct integral_constant {
  static const T value = v;
};
typedef integral_constant<bool, true> true_type;
typedef integral_constant<bool, false> false_type;
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
template <typename T>
struct is_array : false_type {};
template <typename T>
struct is_array<T[]> : true_type {};
template <typename T, size_t N>
struct is_array<T[N]> : true_type {};
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
template <typename TBase, typename TDerived>
class is_base_of {
 protected:  // <- to avoid GCC's "all member functions in class are private"
  typedef char Yes[1];
  typedef char No[2];
  static Yes &probe(const TBase *);
  static No &probe(...);
 public:
  static const bool value =
      sizeof(probe(reinterpret_cast<TDerived *>(0))) == sizeof(Yes);
};
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
template <typename T>
struct is_const : false_type {};
template <typename T>
struct is_const<const T> : true_type {};
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
template <typename>
struct is_floating_point : false_type {};
template <>
struct is_floating_point<float> : true_type {};
template <>
struct is_floating_point<double> : true_type {};
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
template <typename T, typename U>
struct is_same : false_type {};
template <typename T>
struct is_same<T, T> : true_type {};
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
template <typename T>
struct is_integral {
  static const bool value =
      is_same<T, signed char>::value || is_same<T, unsigned char>::value ||
      is_same<T, signed short>::value || is_same<T, unsigned short>::value ||
      is_same<T, signed int>::value || is_same<T, unsigned int>::value ||
      is_same<T, signed long>::value || is_same<T, unsigned long>::value ||
#if ARDUINOJSON_HAS_LONG_LONG
      is_same<T, signed long long>::value ||
      is_same<T, unsigned long long>::value ||
#endif
#if ARDUINOJSON_HAS_INT64
      is_same<T, signed __int64>::value ||
      is_same<T, unsigned __int64>::value ||
#endif
      is_same<T, char>::value;
};
template <typename T>
struct is_integral<const T> : is_integral<T> {};
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
template <typename>
struct is_signed : false_type {};
template <>
struct is_signed<char> : true_type {};
template <>
struct is_signed<signed char> : true_type {};
template <>
struct is_signed<signed short> : true_type {};
template <>
struct is_signed<signed int> : true_type {};
template <>
struct is_signed<signed long> : true_type {};
template <>
struct is_signed<float> : true_type {};
template <>
struct is_signed<double> : true_type {};
#if ARDUINOJSON_HAS_LONG_LONG
template <>
struct is_signed<signed long long> : true_type {};
#endif
#if ARDUINOJSON_HAS_INT64
template <>
struct is_signed<signed __int64> : true_type {};
#endif
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
template <typename>
struct is_unsigned : false_type {};
template <>
struct is_unsigned<bool> : true_type {};
template <>
struct is_unsigned<unsigned char> : true_type {};
template <>
struct is_unsigned<unsigned short> : true_type {};
template <>
struct is_unsigned<unsigned int> : true_type {};
template <>
struct is_unsigned<unsigned long> : true_type {};
#if ARDUINOJSON_HAS_INT64
template <>
struct is_unsigned<unsigned __int64> : true_type {};
#endif
#if ARDUINOJSON_HAS_LONG_LONG
template <>
struct is_unsigned<unsigned long long> : true_type {};
#endif
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
template <typename T>
struct remove_const {
  typedef T type;
};
template <typename T>
struct remove_const<const T> {
  typedef T type;
};
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
template <typename T>
struct remove_reference {
  typedef T type;
};
template <typename T>
struct remove_reference<T&> {
  typedef T type;
};
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
class MemoryPool;
class VariantData;
class VariantSlot;
class CollectionData {
  VariantSlot *_head;
  VariantSlot *_tail;
 public:
  VariantSlot *addSlot(MemoryPool *);
  VariantData *add(MemoryPool *pool);
  template <typename TKey>
  VariantData *add(TKey key, MemoryPool *pool);
  void clear();
  template <typename TKey>
  bool containsKey(const TKey &key) const;
  bool copyFrom(const CollectionData &src, MemoryPool *pool);
  bool equalsArray(const CollectionData &other) const;
  bool equalsObject(const CollectionData &other) const;
  VariantData *get(size_t index) const;
  template <typename TKey>
  VariantData *get(TKey key) const;
  VariantSlot *head() const {
    return _head;
  }
  void remove(size_t index);
  template <typename TKey>
  void remove(TKey key) {
    remove(getSlot(key));
  }
  void remove(VariantSlot *slot);
  size_t size() const;
 private:
  VariantSlot *getSlot(size_t index) const;
  template <typename TKey>
  VariantSlot *getSlot(TKey key) const;
  VariantSlot *getPreviousSlot(VariantSlot *) const;
};
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
#if ARDUINOJSON_USE_DOUBLE
typedef double Float;
#else
typedef float Float;
#endif
}  // namespace ARDUINOJSON_NAMESPACE
#include <stdint.h>  // int64_t
namespace ARDUINOJSON_NAMESPACE {
#if ARDUINOJSON_USE_LONG_LONG
typedef int64_t Integer;
typedef uint64_t UInt;
#else
typedef long Integer;
typedef unsigned long UInt;
#endif
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
enum {
  VALUE_IS_NULL = 0,
  VALUE_IS_LINKED_RAW,
  VALUE_IS_OWNED_RAW,
  VALUE_IS_LINKED_STRING,
  VALUE_IS_OWNED_STRING,
  VALUE_IS_BOOLEAN,
  VALUE_IS_POSITIVE_INTEGER,
  VALUE_IS_NEGATIVE_INTEGER,
  VALUE_IS_ARRAY,
  VALUE_IS_OBJECT,
  VALUE_IS_FLOAT,
  VALUE_MASK = 0x7F,
  KEY_IS_OWNED = 0x80
};
struct RawData {
  const char *data;
  size_t size;
};
union VariantContent {
  Float asFloat;
  UInt asInteger;
  CollectionData asCollection;
  const char *asString;
  struct {
    const char *data;
    size_t size;
  } asRaw;
};
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
typedef conditional<sizeof(void*) <= 2, int8_t, int16_t>::type VariantSlotDiff;
class VariantSlot {
  VariantContent _content;
  uint8_t _flags;
  VariantSlotDiff _next;
  const char* _key;
 public:
  VariantData* data() {
    return reinterpret_cast<VariantData*>(&_content);
  }
  const VariantData* data() const {
    return reinterpret_cast<const VariantData*>(&_content);
  }
  VariantSlot* next() {
    return _next ? this + _next : 0;
  }
  const VariantSlot* next() const {
    return const_cast<VariantSlot*>(this)->next();
  }
  VariantSlot* next(size_t distance) {
    VariantSlot* slot = this;
    while (distance--) {
      if (!slot->_next) return 0;
      slot += slot->_next;
    }
    return slot;
  }
  const VariantSlot* next(size_t distance) const {
    return const_cast<VariantSlot*>(this)->next(distance);
  }
  void setNext(VariantSlot* slot) {
    _next = VariantSlotDiff(slot ? slot - this : 0);
  }
  void setNextNotNull(VariantSlot* slot) {
    ARDUINOJSON_ASSERT(slot != 0);
    _next = VariantSlotDiff(slot - this);
  }
  void setOwnedKey(const char* k) {
    _flags |= KEY_IS_OWNED;
    _key = k;
  }
  void setLinkedKey(const char* k) {
    _flags &= VALUE_MASK;
    _key = k;
  }
  const char* key() const {
    return _key;
  }
  bool ownsKey() const {
    return (_flags & KEY_IS_OWNED) != 0;
  }
  void clear() {
    _next = 0;
    _flags = 0;
    _key = 0;
  }
};
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
inline bool isAligned(void *ptr) {
  const size_t mask = sizeof(void *) - 1;
  size_t addr = reinterpret_cast<size_t>(ptr);
  return (addr & mask) == 0;
}
inline size_t addPadding(size_t bytes) {
  const size_t mask = sizeof(void *) - 1;
  return (bytes + mask) & ~mask;
}
template <size_t bytes>
struct AddPadding {
  static const size_t mask = sizeof(void *) - 1;
  static const size_t value = (bytes + mask) & ~mask;
};
}  // namespace ARDUINOJSON_NAMESPACE
#define JSON_STRING_SIZE(SIZE) (SIZE)
namespace ARDUINOJSON_NAMESPACE {
struct StringSlot {
  char *value;
  size_t size;
};
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
class MemoryPool {
 public:
  MemoryPool(char* buf, size_t capa)
      : _begin(buf),
        _left(buf),
        _right(buf ? buf + capa : 0),
        _end(buf ? buf + capa : 0) {
    ARDUINOJSON_ASSERT(isAligned(_begin));
    ARDUINOJSON_ASSERT(isAligned(_right));
    ARDUINOJSON_ASSERT(isAligned(_end));
  }
  void* buffer() {
    return _begin;
  }
  size_t capacity() const {
    return size_t(_end - _begin);
  }
  size_t size() const {
    return size_t(_left - _begin + _end - _right);
  }
  VariantSlot* allocVariant() {
    return allocRight<VariantSlot>();
  }
  char* allocFrozenString(size_t n) {
    if (!canAlloc(n)) return 0;
    char* s = _left;
    _left += n;
    checkInvariants();
    return s;
  }
  StringSlot allocExpandableString() {
    StringSlot s;
    s.value = _left;
    s.size = size_t(_right - _left);
    _left = _right;
    checkInvariants();
    return s;
  }
  void freezeString(StringSlot& s, size_t newSize) {
    _left -= (s.size - newSize);
    s.size = newSize;
    checkInvariants();
  }
  void clear() {
    _left = _begin;
    _right = _end;
  }
  bool canAlloc(size_t bytes) const {
    return _left + bytes <= _right;
  }
  bool owns(void* p) const {
    return _begin <= p && p < _end;
  }
  template <typename T>
  T* allocRight() {
    return reinterpret_cast<T*>(allocRight(sizeof(T)));
  }
  char* allocRight(size_t bytes) {
    if (!canAlloc(bytes)) return 0;
    _right -= bytes;
    return _right;
  }
  void* operator new(size_t, void* p) {
    return p;
  }
 private:
  StringSlot* allocStringSlot() {
    return allocRight<StringSlot>();
  }
  void checkInvariants() {
    ARDUINOJSON_ASSERT(_begin <= _left);
    ARDUINOJSON_ASSERT(_left <= _right);
    ARDUINOJSON_ASSERT(_right <= _end);
    ARDUINOJSON_ASSERT(isAligned(_right));
  }
  char *_begin, *_left, *_right, *_end;
};
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
template <typename>
struct IsString : false_type {};
template <typename T>
struct IsString<const T> : IsString<T> {};
template <typename T>
struct IsString<T&> : IsString<T> {};
}  // namespace ARDUINOJSON_NAMESPACE
#include <string.h>  // strcmp
namespace ARDUINOJSON_NAMESPACE {
class ConstRamStringWrapper {
 public:
  ConstRamStringWrapper(const char* str = 0) : _str(str) {}
  bool equals(const char* expected) const {
    const char* actual = _str;
    if (!actual || !expected) return actual == expected;
    return strcmp(actual, expected) == 0;
  }
  bool isNull() const {
    return !_str;
  }
  size_t size() const {
    return strlen(_str);
  }
  const char* c_str() const {
    return _str;
  }
 protected:
  const char* _str;
};
inline ConstRamStringWrapper wrapString(const char* str) {
  return ConstRamStringWrapper(str);
}
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
class RamStringWrapper : public ConstRamStringWrapper {
 public:
  RamStringWrapper(const char* str) : ConstRamStringWrapper(str) {}
  char* save(MemoryPool* pool) const {
    if (!_str) return NULL;
    size_t n = size() + 1;
    char* dup = pool->allocFrozenString(n);
    if (dup) memcpy(dup, _str, n);
    return dup;
  }
};
template <typename TChar>
inline RamStringWrapper wrapString(const TChar* str) {
  return RamStringWrapper(reinterpret_cast<const char*>(str));
}
inline RamStringWrapper wrapString(char* str) {
  return RamStringWrapper(str);
}
template <typename TChar>
struct IsString<TChar*> {
  static const bool value = sizeof(TChar) == 1;
};
template <>
struct IsString<void*> {
  static const bool value = false;
};
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
class SizedRamStringWrapper {
 public:
  SizedRamStringWrapper(const char* str, size_t n) : _str(str), _size(n) {}
  bool equals(const char* expected) const {
    const char* actual = reinterpret_cast<const char*>(_str);
    if (!actual || !expected) return actual == expected;
    return strcmp(actual, expected) == 0;
  }
  bool isNull() const {
    return !_str;
  }
  char* save(MemoryPool* pool) const {
    if (!_str) return NULL;
    char* dup = pool->allocFrozenString(_size);
    if (dup) memcpy(dup, _str, _size);
    return dup;
  }
  size_t size() const {
    return strlen(reinterpret_cast<const char*>(_str));
  }
 private:
  const char* _str;
  size_t _size;
};
template <typename TChar>
inline SizedRamStringWrapper wrapString(const TChar* str, size_t size) {
  return SizedRamStringWrapper(reinterpret_cast<const char*>(str), size);
}
}  // namespace ARDUINOJSON_NAMESPACE
#if ARDUINOJSON_ENABLE_STD_STRING
#include <string>
namespace ARDUINOJSON_NAMESPACE {
class StlStringWrapper {
 public:
  StlStringWrapper(const std::string& str) : _str(&str) {}
  char* save(MemoryPool* pool) const {
    size_t n = _str->length() + 1;
    char* dup = pool->allocFrozenString(n);
    if (dup) memcpy(dup, _str->c_str(), n);
    return dup;
  }
  bool isNull() const {
    return false;
  }
  bool equals(const char* expected) const {
    if (!expected) return false;
    return *_str == expected;
  }
  const char* data() const {
    return _str->data();
  }
  size_t size() const {
    return _str->size();
  }
 private:
  const std::string* _str;
};
template <>
struct IsString<std::string> : true_type {};
inline StlStringWrapper wrapString(const std::string& str) {
  return StlStringWrapper(str);
}
}  // namespace ARDUINOJSON_NAMESPACE
#endif
#if ARDUINOJSON_ENABLE_ARDUINO_STRING
#include <WString.h>
namespace ARDUINOJSON_NAMESPACE {
class ArduinoStringWrapper {
 public:
  ArduinoStringWrapper(const ::String& str) : _str(&str) {}
  char* save(MemoryPool* pool) const {
    if (isNull()) return NULL;
    size_t n = _str->length() + 1;
    char* dup = pool->allocFrozenString(n);
    if (dup) memcpy(dup, _str->c_str(), n);
    return dup;
  }
  bool isNull() const {
    return !_str->c_str();
  }
  bool equals(const char* expected) const {
    const char* actual = _str->c_str();
    if (!actual || !expected) return actual == expected;
    return 0 == strcmp(actual, expected);
  }
  const char* data() const {
    return _str->c_str();
  }
  size_t size() const {
    return _str->length();
  }
 private:
  const ::String* _str;
};
template <>
struct IsString< ::String> : true_type {};
template <>
struct IsString< ::StringSumHelper> : true_type {};
inline ArduinoStringWrapper wrapString(const ::String& str) {
  return ArduinoStringWrapper(str);
}
}  // namespace ARDUINOJSON_NAMESPACE
#endif
#if ARDUINOJSON_ENABLE_PROGMEM
namespace ARDUINOJSON_NAMESPACE {
class FlashStringWrapper {
 public:
  FlashStringWrapper(const __FlashStringHelper* str) : _str(str) {}
  bool equals(const char* expected) const {
    const char* actual = reinterpret_cast<const char*>(_str);
    if (!actual || !expected) return actual == expected;
    return strcmp_P(expected, actual) == 0;
  }
  bool isNull() const {
    return !_str;
  }
  char* save(MemoryPool* pool) const {
    if (!_str) return NULL;
    size_t n = size() + 1;  // copy the terminator
    char* dup = pool->allocFrozenString(n);
    if (dup) memcpy_P(dup, reinterpret_cast<const char*>(_str), n);
    return dup;
  }
  size_t size() const {
    return strlen_P(reinterpret_cast<const char*>(_str));
  }
 private:
  const __FlashStringHelper* _str;
};
inline FlashStringWrapper wrapString(const __FlashStringHelper* str) {
  return FlashStringWrapper(str);
}
template <>
struct IsString<const __FlashStringHelper*> : true_type {};
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
class SizedFlashStringWrapper {
 public:
  SizedFlashStringWrapper(const __FlashStringHelper* str, size_t sz)
      : _str(str), _size(sz) {}
  bool equals(const char* expected) const {
    const char* actual = reinterpret_cast<const char*>(_str);
    if (!actual || !expected) return actual == expected;
    return strncmp_P(expected, actual, _size) == 0;
  }
  bool isNull() const {
    return !_str;
  }
  char* save(MemoryPool* pool) const {
    if (!_str) return NULL;
    char* dup = pool->allocFrozenString(_size);
    if (!dup) memcpy_P(dup, (const char*)_str, _size);
    return dup;
  }
  size_t size() const {
    return strlen_P(reinterpret_cast<const char*>(_str));
  }
 private:
  const __FlashStringHelper* _str;
  size_t _size;
};
inline SizedFlashStringWrapper wrapString(const __FlashStringHelper* str,
                                          size_t sz) {
  return SizedFlashStringWrapper(str, sz);
}
}  // namespace ARDUINOJSON_NAMESPACE
#endif
namespace ARDUINOJSON_NAMESPACE {
template <typename T>
class SerializedValue {
 public:
  explicit SerializedValue(T str) : _str(str) {}
  operator T() const {
    return _str;
  }
  const char* data() const {
    return _str.c_str();
  }
  size_t size() const {
    return _str.length();
  }
 private:
  T _str;
};
template <typename TChar>
class SerializedValue<TChar*> {
 public:
  explicit SerializedValue(TChar* p, size_t n) : _data(p), _size(n) {}
  operator TChar*() const {
    return _data;
  }
  TChar* data() const {
    return _data;
  }
  size_t size() const {
    return _size;
  }
 private:
  TChar* _data;
  size_t _size;
};
template <typename T>
inline SerializedValue<T> serialized(T str) {
  return SerializedValue<T>(str);
}
template <typename TChar>
inline SerializedValue<TChar*> serialized(TChar* p) {
  return SerializedValue<TChar*>(p, wrapString(p).size());
}
template <typename TChar>
inline SerializedValue<TChar*> serialized(TChar* p, size_t n) {
  return SerializedValue<TChar*>(p, n);
}
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
class VariantData {
  VariantContent _content;  // must be first to allow cast from array to variant
  uint8_t _flags;
 public:
  template <typename Visitor>
  void accept(Visitor &visitor) const {
    switch (type()) {
      case VALUE_IS_FLOAT:
        return visitor.visitFloat(_content.asFloat);
      case VALUE_IS_ARRAY:
        return visitor.visitArray(_content.asCollection);
      case VALUE_IS_OBJECT:
        return visitor.visitObject(_content.asCollection);
      case VALUE_IS_LINKED_STRING:
      case VALUE_IS_OWNED_STRING:
        return visitor.visitString(_content.asString);
      case VALUE_IS_OWNED_RAW:
      case VALUE_IS_LINKED_RAW:
        return visitor.visitRawJson(_content.asRaw.data, _content.asRaw.size);
      case VALUE_IS_NEGATIVE_INTEGER:
        return visitor.visitNegativeInteger(_content.asInteger);
      case VALUE_IS_POSITIVE_INTEGER:
        return visitor.visitPositiveInteger(_content.asInteger);
      case VALUE_IS_BOOLEAN:
        return visitor.visitBoolean(_content.asInteger != 0);
      default:
        return visitor.visitNull();
    }
  }
  template <typename T>
  T asIntegral() const;
  template <typename T>
  T asFloat() const;
  const char *asString() const;
  bool asBoolean() const {
    return asIntegral<int>() != 0;
  }
  CollectionData *asArray() {
    if (type() == VALUE_IS_ARRAY)
      return &_content.asCollection;
    else
      return 0;
  }
  const CollectionData *asArray() const {
    return const_cast<VariantData *>(this)->asArray();
  }
  CollectionData *asObject() {
    if (type() == VALUE_IS_OBJECT)
      return &_content.asCollection;
    else
      return 0;
  }
  const CollectionData *asObject() const {
    return const_cast<VariantData *>(this)->asObject();
  }
  bool copyFrom(const VariantData &src, MemoryPool *pool) {
    switch (src.type()) {
      case VALUE_IS_ARRAY:
        return toArray().copyFrom(src._content.asCollection, pool);
      case VALUE_IS_OBJECT:
        return toObject().copyFrom(src._content.asCollection, pool);
      case VALUE_IS_OWNED_STRING:
        return setOwnedString(RamStringWrapper(src._content.asString), pool);
      case VALUE_IS_OWNED_RAW:
        return setOwnedRaw(
            serialized(src._content.asRaw.data, src._content.asRaw.size), pool);
      default:
        setType(src.type());
        _content = src._content;
        return true;
    }
  }
  bool equals(const VariantData &other) const {
    if (type() != other.type()) return false;
    switch (type()) {
      case VALUE_IS_LINKED_STRING:
      case VALUE_IS_OWNED_STRING:
        return !strcmp(_content.asString, other._content.asString);
      case VALUE_IS_LINKED_RAW:
      case VALUE_IS_OWNED_RAW:
        return _content.asRaw.size == other._content.asRaw.size &&
               !memcmp(_content.asRaw.data, other._content.asRaw.data,
                       _content.asRaw.size);
      case VALUE_IS_BOOLEAN:
      case VALUE_IS_POSITIVE_INTEGER:
      case VALUE_IS_NEGATIVE_INTEGER:
        return _content.asInteger == other._content.asInteger;
      case VALUE_IS_ARRAY:
        return _content.asCollection.equalsArray(other._content.asCollection);
      case VALUE_IS_OBJECT:
        return _content.asCollection.equalsObject(other._content.asCollection);
      case VALUE_IS_FLOAT:
        return _content.asFloat == other._content.asFloat;
      case VALUE_IS_NULL:
      default:
        return true;
    }
  }
  bool isArray() const {
    return type() == VALUE_IS_ARRAY;
  }
  bool isBoolean() const {
    return type() == VALUE_IS_BOOLEAN;
  }
  bool isInteger() const {
    return type() == VALUE_IS_POSITIVE_INTEGER ||
           type() == VALUE_IS_NEGATIVE_INTEGER;
  }
  bool isFloat() const {
    return type() == VALUE_IS_FLOAT || type() == VALUE_IS_POSITIVE_INTEGER ||
           type() == VALUE_IS_NEGATIVE_INTEGER;
  }
  bool isString() const {
    return (type() == VALUE_IS_LINKED_STRING ||
            type() == VALUE_IS_OWNED_STRING);
  }
  bool isObject() const {
    return type() == VALUE_IS_OBJECT;
  }
  bool isNull() const {
    return type() == VALUE_IS_NULL;
  }
  void setBoolean(bool value) {
    setType(VALUE_IS_BOOLEAN);
    _content.asInteger = static_cast<UInt>(value);
  }
  void setFloat(Float value) {
    setType(VALUE_IS_FLOAT);
    _content.asFloat = value;
  }
  void setLinkedRaw(SerializedValue<const char *> value) {
    setType(VALUE_IS_LINKED_RAW);
    _content.asRaw.data = value.data();
    _content.asRaw.size = value.size();
  }
  template <typename T>
  bool setOwnedRaw(SerializedValue<T> value, MemoryPool *pool) {
    char *dup = wrapString(value.data(), value.size()).save(pool);
    if (dup) {
      setType(VALUE_IS_OWNED_RAW);
      _content.asRaw.data = dup;
      _content.asRaw.size = value.size();
      return true;
    } else {
      setType(VALUE_IS_NULL);
      return false;
    }
  }
  template <typename T>
  typename enable_if<is_unsigned<T>::value>::type setInteger(T value) {
    setUnsignedInteger(value);
  }
  template <typename T>
  typename enable_if<is_signed<T>::value>::type setInteger(T value) {
    setSignedInteger(value);
  }
  template <typename T>
  void setSignedInteger(T value) {
    if (value >= 0) {
      setType(VALUE_IS_POSITIVE_INTEGER);
      _content.asInteger = static_cast<UInt>(value);
    } else {
      setType(VALUE_IS_NEGATIVE_INTEGER);
      _content.asInteger = ~static_cast<UInt>(value) + 1;
    }
  }
  void setLinkedString(const char *value) {
    setType(VALUE_IS_LINKED_STRING);
    _content.asString = value;
  }
  void setNull() {
    setType(VALUE_IS_NULL);
  }
  void setOwnedString(const char *s) {
    setType(VALUE_IS_OWNED_STRING);
    _content.asString = s;
  }
  template <typename T>
  bool setOwnedString(T value, MemoryPool *pool) {
    char *dup = value.save(pool);
    if (dup) {
      setType(VALUE_IS_OWNED_STRING);
      _content.asString = dup;
      return true;
    } else {
      setType(VALUE_IS_NULL);
      return false;
    }
  }
  void setUnsignedInteger(UInt value) {
    setType(VALUE_IS_POSITIVE_INTEGER);
    _content.asInteger = static_cast<UInt>(value);
  }
  CollectionData &toArray() {
    setType(VALUE_IS_ARRAY);
    _content.asCollection.clear();
    return _content.asCollection;
  }
  CollectionData &toObject() {
    setType(VALUE_IS_OBJECT);
    _content.asCollection.clear();
    return _content.asCollection;
  }
  size_t size() const {
    if (type() == VALUE_IS_OBJECT || type() == VALUE_IS_ARRAY)
      return _content.asCollection.size();
    else
      return 0;
  }
 private:
  uint8_t type() const {
    return _flags & VALUE_MASK;
  }
  void setType(uint8_t t) {
    _flags &= KEY_IS_OWNED;
    _flags |= t;
  }
};
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
inline VariantData *arrayAdd(CollectionData *arr, MemoryPool *pool) {
  return arr ? arr->add(pool) : 0;
}
template <typename Visitor>
inline void arrayAccept(const CollectionData *arr, Visitor &visitor) {
  if (arr)
    visitor.visitArray(*arr);
  else
    visitor.visitNull();
}
inline bool arrayEquals(const CollectionData *lhs, const CollectionData *rhs) {
  if (lhs == rhs) return true;
  if (!lhs || !rhs) return false;
  return lhs->equalsArray(*rhs);
}
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
template <typename TKey>
inline bool slotSetKey(VariantSlot* var, TKey key, MemoryPool* pool) {
  if (!var) return false;
  char* dup = key.save(pool);
  if (!dup) return false;
  var->setOwnedKey(dup);
  return true;
}
inline bool slotSetKey(VariantSlot* var, ConstRamStringWrapper key,
                       MemoryPool*) {
  if (!var) return false;
  var->setLinkedKey(key.c_str());
  return true;
}
inline size_t slotSize(const VariantSlot* var) {
  size_t n = 0;
  while (var) {
    n++;
    var = var->next();
  }
  return n;
}
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
struct Visitable {
};
template <typename T>
struct IsVisitable : is_base_of<Visitable, T> {};
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
#ifndef isnan
template <typename T>
bool isnan(T x) {
  return x != x;
}
#endif
#ifndef isinf
template <typename T>
bool isinf(T x) {
  return x != 0.0 && x * 2 == x;
}
#endif
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
template <typename T, size_t = sizeof(T)>
struct FloatTraits {};
template <typename T>
struct FloatTraits<T, 8 /*64bits*/> {
  typedef int64_t mantissa_type;
  static const short mantissa_bits = 52;
  static const mantissa_type mantissa_max =
      (static_cast<mantissa_type>(1) << mantissa_bits) - 1;
  typedef int16_t exponent_type;
  static const exponent_type exponent_max = 308;
  template <typename TExponent>
  static T make_float(T m, TExponent e) {
    if (e > 0) {
      for (uint8_t index = 0; e != 0; index++) {
        if (e & 1) m *= positiveBinaryPowerOfTen(index);
        e >>= 1;
      }
    } else {
      e = TExponent(-e);
      for (uint8_t index = 0; e != 0; index++) {
        if (e & 1) m *= negativeBinaryPowerOfTen(index);
        e >>= 1;
      }
    }
    return m;
  }
  static T positiveBinaryPowerOfTen(int index) {
    static T factors[] = {
        1e1,
        1e2,
        1e4,
        1e8,
        1e16,
        forge(0x4693B8B5, 0xB5056E17),  // 1e32
        forge(0x4D384F03, 0xE93FF9F5),  // 1e64
        forge(0x5A827748, 0xF9301D32),  // 1e128
        forge(0x75154FDD, 0x7F73BF3C)   // 1e256
    };
    return factors[index];
  }
  static T negativeBinaryPowerOfTen(int index) {
    static T factors[] = {
        forge(0x3FB99999, 0x9999999A),  // 1e-1
        forge(0x3F847AE1, 0x47AE147B),  // 1e-2
        forge(0x3F1A36E2, 0xEB1C432D),  // 1e-4
        forge(0x3E45798E, 0xE2308C3A),  // 1e-8
        forge(0x3C9CD2B2, 0x97D889BC),  // 1e-16
        forge(0x3949F623, 0xD5A8A733),  // 1e-32
        forge(0x32A50FFD, 0x44F4A73D),  // 1e-64
        forge(0x255BBA08, 0xCF8C979D),  // 1e-128
        forge(0x0AC80628, 0x64AC6F43)   // 1e-256
    };
    return factors[index];
  }
  static T negativeBinaryPowerOfTenPlusOne(int index) {
    static T factors[] = {
        1e0,
        forge(0x3FB99999, 0x9999999A),  // 1e-1
        forge(0x3F50624D, 0xD2F1A9FC),  // 1e-3
        forge(0x3E7AD7F2, 0x9ABCAF48),  // 1e-7
        forge(0x3CD203AF, 0x9EE75616),  // 1e-15
        forge(0x398039D6, 0x65896880),  // 1e-31
        forge(0x32DA53FC, 0x9631D10D),  // 1e-63
        forge(0x25915445, 0x81B7DEC2),  // 1e-127
        forge(0x0AFE07B2, 0x7DD78B14)   // 1e-255
    };
    return factors[index];
  }
  static T nan() {
    return forge(0x7ff80000, 0x00000000);
  }
  static T inf() {
    return forge(0x7ff00000, 0x00000000);
  }
  static T forge(uint32_t msb, uint32_t lsb) {
    union {
      uint64_t integerBits;
      T floatBits;
    };
    integerBits = (uint64_t(msb) << 32) | lsb;
    return floatBits;
  }
};
template <typename T>
struct FloatTraits<T, 4 /*32bits*/> {
  typedef int32_t mantissa_type;
  static const short mantissa_bits = 23;
  static const mantissa_type mantissa_max =
      (static_cast<mantissa_type>(1) << mantissa_bits) - 1;
  typedef int8_t exponent_type;
  static const exponent_type exponent_max = 38;
  template <typename TExponent>
  static T make_float(T m, TExponent e) {
    if (e > 0) {
      for (uint8_t index = 0; e != 0; index++) {
        if (e & 1) m *= positiveBinaryPowerOfTen(index);
        e >>= 1;
      }
    } else {
      e = -e;
      for (uint8_t index = 0; e != 0; index++) {
        if (e & 1) m *= negativeBinaryPowerOfTen(index);
        e >>= 1;
      }
    }
    return m;
  }
  static T positiveBinaryPowerOfTen(int index) {
    static T factors[] = {1e1f, 1e2f, 1e4f, 1e8f, 1e16f, 1e32f};
    return factors[index];
  }
  static T negativeBinaryPowerOfTen(int index) {
    static T factors[] = {1e-1f, 1e-2f, 1e-4f, 1e-8f, 1e-16f, 1e-32f};
    return factors[index];
  }
  static T negativeBinaryPowerOfTenPlusOne(int index) {
    static T factors[] = {1e0f, 1e-1f, 1e-3f, 1e-7f, 1e-15f, 1e-31f};
    return factors[index];
  }
  static T forge(uint32_t bits) {
    union {
      uint32_t integerBits;
      T floatBits;
    };
    integerBits = bits;
    return floatBits;
  }
  static T nan() {
    return forge(0x7fc00000);
  }
  static T inf() {
    return forge(0x7f800000);
  }
};
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
inline bool isdigit(char c) {
  return '0' <= c && c <= '9';
}
inline bool issign(char c) {
  return '-' == c || c == '+';
}
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
template <typename T>
inline T parseFloat(const char* s) {
  typedef FloatTraits<T> traits;
  typedef typename traits::mantissa_type mantissa_t;
  typedef typename traits::exponent_type exponent_t;
  if (!s) return 0;  // NULL
  bool negative_result = false;
  switch (*s) {
    case '-':
      negative_result = true;
      s++;
      break;
    case '+':
      s++;
      break;
  }
  if (*s == 't') return 1;  // true
  if (*s == 'n' || *s == 'N') return traits::nan();
  if (*s == 'i' || *s == 'I')
    return negative_result ? -traits::inf() : traits::inf();
  mantissa_t mantissa = 0;
  exponent_t exponent_offset = 0;
  while (isdigit(*s)) {
    if (mantissa < traits::mantissa_max / 10)
      mantissa = mantissa * 10 + (*s - '0');
    else
      exponent_offset++;
    s++;
  }
  if (*s == '.') {
    s++;
    while (isdigit(*s)) {
      if (mantissa < traits::mantissa_max / 10) {
        mantissa = mantissa * 10 + (*s - '0');
        exponent_offset--;
      }
      s++;
    }
  }
  int exponent = 0;
  if (*s == 'e' || *s == 'E') {
    s++;
    bool negative_exponent = false;
    if (*s == '-') {
      negative_exponent = true;
      s++;
    } else if (*s == '+') {
      s++;
    }
    while (isdigit(*s)) {
      exponent = exponent * 10 + (*s - '0');
      if (exponent + exponent_offset > traits::exponent_max) {
        if (negative_exponent)
          return negative_result ? -0.0f : 0.0f;
        else
          return negative_result ? -traits::inf() : traits::inf();
      }
      s++;
    }
    if (negative_exponent) exponent = -exponent;
  }
  exponent += exponent_offset;
  T result = traits::make_float(static_cast<T>(mantissa), exponent);
  return negative_result ? -result : result;
}
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
template <typename T>
T parseInteger(const char *s) {
  if (!s) return 0;  // NULL
  if (*s == 't') return 1;  // "true"
  T result = 0;
  bool negative_result = false;
  switch (*s) {
    case '-':
      negative_result = true;
      s++;
      break;
    case '+':
      s++;
      break;
  }
  while (isdigit(*s)) {
    result = T(result * 10 + T(*s - '0'));
    s++;
  }
  return negative_result ? T(~result + 1) : result;
}
}  // namespace ARDUINOJSON_NAMESPACE
#ifdef _MSC_VER  // Visual Studio
#define FORCE_INLINE  // __forceinline causes C4714 when returning std::string
#define NO_INLINE __declspec(noinline)
#define DEPRECATED(msg) __declspec(deprecated(msg))
#elif defined(__GNUC__)  // GCC or Clang
#define FORCE_INLINE __attribute__((always_inline))
#define NO_INLINE __attribute__((noinline))
#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 5)
#define DEPRECATED(msg) __attribute__((deprecated(msg)))
#else
#define DEPRECATED(msg) __attribute__((deprecated))
#endif
#else  // Other compilers
#define FORCE_INLINE
#define NO_INLINE
#define DEPRECATED(msg)
#endif
#if __cplusplus >= 201103L
#define NOEXCEPT noexcept
#else
#define NOEXCEPT throw()
#endif
namespace ARDUINOJSON_NAMESPACE {
template <typename TImpl>
class VariantCasts {
 public:
  template <typename T>
  FORCE_INLINE operator T() const {
    return impl()->template as<T>();
  }
 private:
  const TImpl *impl() const {
    return static_cast<const TImpl *>(this);
  }
};
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
template <typename T>
struct is_simple_value {
  static const bool value = is_integral<T>::value ||
                            is_floating_point<T>::value ||
                            is_same<T, bool>::value;
};
template <typename TVariant>
class VariantComparisons {
 public:
  template <typename T>
  friend typename enable_if<IsString<T *>::value, bool>::type operator==(
      T *lhs, TVariant rhs) {
    return wrapString(lhs).equals(rhs.template as<const char *>());
  }
  template <typename T>
  friend typename enable_if<IsString<T>::value, bool>::type operator==(
      const T &lhs, TVariant rhs) {
    return wrapString(lhs).equals(rhs.template as<const char *>());
  }
  template <typename T>
  friend typename enable_if<IsString<T *>::value, bool>::type operator==(
      TVariant lhs, T *rhs) {
    return wrapString(rhs).equals(lhs.template as<const char *>());
  }
  template <typename T>
  friend typename enable_if<IsString<T>::value, bool>::type operator==(
      TVariant lhs, const T &rhs) {
    return wrapString(rhs).equals(lhs.template as<const char *>());
  }
  template <typename T>
  friend typename enable_if<is_simple_value<T>::value, bool>::type operator==(
      const T &lhs, TVariant rhs) {
    return lhs == rhs.template as<T>();
  }
  template <typename T>
  friend typename enable_if<is_simple_value<T>::value, bool>::type operator==(
      TVariant lhs, const T &rhs) {
    return lhs.template as<T>() == rhs;
  }
  template <typename T>
  friend typename enable_if<IsString<T *>::value, bool>::type operator!=(
      T *lhs, TVariant rhs) {
    return !wrapString(lhs).equals(rhs.template as<const char *>());
  }
  template <typename T>
  friend typename enable_if<IsString<T>::value, bool>::type operator!=(
      const T &lhs, TVariant rhs) {
    return !wrapString(lhs).equals(rhs.template as<const char *>());
  }
  template <typename T>
  friend typename enable_if<IsString<T *>::value, bool>::type operator!=(
      TVariant lhs, T *rhs) {
    return !wrapString(rhs).equals(lhs.template as<const char *>());
  }
  template <typename T>
  friend typename enable_if<IsString<T>::value, bool>::type operator!=(
      TVariant lhs, const T &rhs) {
    return !wrapString(rhs).equals(lhs.template as<const char *>());
  }
  template <typename T>
  friend typename enable_if<is_simple_value<T>::value, bool>::type operator!=(
      const T &lhs, TVariant rhs) {
    return lhs != rhs.template as<T>();
  }
  template <typename T>
  friend typename enable_if<is_simple_value<T>::value, bool>::type operator!=(
      TVariant lhs, const T &rhs) {
    return lhs.template as<T>() != rhs;
  }
  template <typename T>
  friend typename enable_if<is_simple_value<T>::value, bool>::type operator<(
      const T &lhs, TVariant rhs) {
    return lhs < rhs.template as<T>();
  }
  template <typename T>
  friend typename enable_if<is_simple_value<T>::value, bool>::type operator<(
      TVariant lhs, const T &rhs) {
    return lhs.template as<T>() < rhs;
  }
  template <typename T>
  friend typename enable_if<is_simple_value<T>::value, bool>::type operator<=(
      const T &lhs, TVariant rhs) {
    return lhs <= rhs.template as<T>();
  }
  template <typename T>
  friend typename enable_if<is_simple_value<T>::value, bool>::type operator<=(
      TVariant lhs, const T &rhs) {
    return lhs.template as<T>() <= rhs;
  }
  template <typename T>
  friend typename enable_if<is_simple_value<T>::value, bool>::type operator>(
      const T &lhs, TVariant rhs) {
    return lhs > rhs.template as<T>();
  }
  template <typename T>
  friend typename enable_if<is_simple_value<T>::value, bool>::type operator>(
      TVariant lhs, const T &rhs) {
    return lhs.template as<T>() > rhs;
  }
  template <typename T>
  friend typename enable_if<is_simple_value<T>::value, bool>::type operator>=(
      const T &lhs, TVariant rhs) {
    return lhs >= rhs.template as<T>();
  }
  template <typename T>
  friend typename enable_if<is_simple_value<T>::value, bool>::type operator>=(
      TVariant lhs, const T &rhs) {
    return lhs.template as<T>() >= rhs;
  }
};
}  // namespace ARDUINOJSON_NAMESPACE
#if ARDUINOJSON_ENABLE_ARDUINO_STRING
#endif
#if ARDUINOJSON_ENABLE_STD_STRING
#endif
namespace ARDUINOJSON_NAMESPACE {
template <typename>
struct IsWriteableString : false_type {};
template <typename TString>
class DynamicStringWriter {};
#if ARDUINOJSON_ENABLE_ARDUINO_STRING
template <>
struct IsWriteableString<String> : true_type {};
template <>
class DynamicStringWriter<String> {
 public:
  DynamicStringWriter(String &str) : _str(&str) {}
  size_t write(uint8_t c) {
    _str->operator+=(static_cast<char>(c));
    return 1;
  }
  size_t write(const uint8_t *s, size_t n) {
    _str->reserve(_str->length() + n);
    while (n > 0) {
      _str->operator+=(static_cast<char>(*s++));
      n--;
    }
    return n;
  }
 private:
  String *_str;
};
#endif
#if ARDUINOJSON_ENABLE_STD_STRING
template <>
struct IsWriteableString<std::string> : true_type {};
template <>
class DynamicStringWriter<std::string> {
 public:
  DynamicStringWriter(std::string &str) : _str(&str) {}
  size_t write(uint8_t c) {
    _str->operator+=(static_cast<char>(c));
    return 1;
  }
  size_t write(const uint8_t *s, size_t n) {
    _str->append(reinterpret_cast<const char *>(s), n);
    return n;
  }
 private:
  std::string *_str;
};
#endif
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
class ArrayRef;
class ArrayConstRef;
class ObjectRef;
class ObjectConstRef;
class VariantRef;
class VariantConstRef;
template <typename T>
struct VariantAs {
  typedef T type;
};
template <>
struct VariantAs<char*> {
  typedef const char* type;
};
template <typename T>
struct VariantConstAs {
  typedef typename VariantAs<T>::type type;
};
template <>
struct VariantConstAs<VariantRef> {
  typedef VariantConstRef type;
};
template <>
struct VariantConstAs<ObjectRef> {
  typedef ObjectConstRef type;
};
template <>
struct VariantConstAs<ArrayRef> {
  typedef ArrayConstRef type;
};
template <typename T>
inline typename enable_if<is_integral<T>::value, T>::type variantAs(
    const VariantData* _data) {
  return _data != 0 ? _data->asIntegral<T>() : T(0);
}
template <typename T>
inline typename enable_if<is_same<T, bool>::value, T>::type variantAs(
    const VariantData* _data) {
  return _data != 0 ? _data->asBoolean() : false;
}
template <typename T>
inline typename enable_if<is_floating_point<T>::value, T>::type variantAs(
    const VariantData* _data) {
  return _data != 0 ? _data->asFloat<T>() : T(0);
}
template <typename T>
inline typename enable_if<is_same<T, const char*>::value ||
                              is_same<T, char*>::value,
                          const char*>::type
variantAs(const VariantData* _data) {
  return _data != 0 ? _data->asString() : 0;
}
template <typename T>
inline typename enable_if<is_same<VariantConstRef, T>::value, T>::type
variantAs(const VariantData* _data);
template <typename T>
inline typename enable_if<IsWriteableString<T>::value, T>::type variantAs(
    const VariantData* _data);
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
template <typename TImpl>
class VariantOr {
 public:
  template <typename T>
  typename enable_if<!is_integral<T>::value, T>::type operator|(
      const T &defaultValue) const {
    if (impl()->template is<T>())
      return impl()->template as<T>();
    else
      return defaultValue;
  }
  const char *operator|(const char *defaultValue) const {
    const char *value = impl()->template as<const char *>();
    return value ? value : defaultValue;
  }
  template <typename Integer>
  typename enable_if<is_integral<Integer>::value, Integer>::type operator|(
      const Integer &defaultValue) const {
    if (impl()->template is<double>())
      return impl()->template as<Integer>();
    else
      return defaultValue;
  }
 private:
  const TImpl *impl() const {
    return static_cast<const TImpl *>(this);
  }
};
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
class ArrayRef;
class ObjectRef;
class ArraySubscript;
template <typename TKey>
class ObjectSubscript;
template <typename TImpl>
class VariantSubscripts {
 public:
  FORCE_INLINE ArraySubscript operator[](size_t index) const;
  template <typename TString>
  FORCE_INLINE typename enable_if<IsString<TString>::value,
                                  ObjectSubscript<const TString &> >::type
  operator[](const TString &key) const;
  template <typename TString>
  FORCE_INLINE typename enable_if<IsString<TString *>::value,
                                  ObjectSubscript<TString *> >::type
  operator[](TString *key) const;
 private:
  const TImpl *impl() const {
    return static_cast<const TImpl *>(this);
  }
};
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
template <typename TImpl>
class VariantOperators : public VariantCasts<TImpl>,
                         public VariantComparisons<TImpl>,
                         public VariantOr<TImpl>,
                         public VariantSubscripts<TImpl> {};
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
template <typename Visitor>
inline void variantAccept(const VariantData *var, Visitor &visitor) {
  if (var != 0)
    var->accept(visitor);
  else
    visitor.visitNull();
}
inline const CollectionData *variantAsObject(const VariantData *var) {
  return var != 0 ? var->asObject() : 0;
}
inline CollectionData *variantAsObject(VariantData *var) {
  return var != 0 ? var->asObject() : 0;
}
inline bool variantCopyFrom(VariantData *dst, const VariantData *src,
                            MemoryPool *pool) {
  if (!dst) return false;
  if (!src) {
    dst->setNull();
    return true;
  }
  return dst->copyFrom(*src, pool);
}
inline bool variantEquals(const VariantData *a, const VariantData *b) {
  if (a == b) return true;
  if (!a || !b) return false;
  return a->equals(*b);
}
inline bool variantIsArray(const VariantData *var) {
  return var && var->isArray();
}
inline bool variantIsBoolean(const VariantData *var) {
  return var && var->isBoolean();
}
inline bool variantIsInteger(const VariantData *var) {
  return var && var->isInteger();
}
inline bool variantIsFloat(const VariantData *var) {
  return var && var->isFloat();
}
inline bool variantIsString(const VariantData *var) {
  return var && var->isString();
}
inline bool variantIsObject(const VariantData *var) {
  return var && var->isObject();
}
inline bool variantIsNull(const VariantData *var) {
  return var == 0 || var->isNull();
}
inline bool variantSetBoolean(VariantData *var, bool value) {
  if (!var) return false;
  var->setBoolean(value);
  return true;
}
inline bool variantSetFloat(VariantData *var, Float value) {
  if (!var) return false;
  var->setFloat(value);
  return true;
}
inline bool variantSetLinkedRaw(VariantData *var,
                                SerializedValue<const char *> value) {
  if (!var) return false;
  var->setLinkedRaw(value);
  return true;
}
template <typename T>
inline bool variantSetOwnedRaw(VariantData *var, SerializedValue<T> value,
                               MemoryPool *pool) {
  return var != 0 && var->setOwnedRaw(value, pool);
}
template <typename T>
inline bool variantSetSignedInteger(VariantData *var, T value) {
  if (!var) return false;
  var->setSignedInteger(value);
  return true;
}
inline bool variantSetLinkedString(VariantData *var, const char *value) {
  if (!var) return false;
  var->setLinkedString(value);
  return true;
}
inline void variantSetNull(VariantData *var) {
  if (!var) return;
  var->setNull();
}
inline bool variantSetOwnedString(VariantData *var, char *value) {
  if (!var) return false;
  var->setOwnedString(value);
  return true;
}
template <typename T>
inline bool variantSetOwnedString(VariantData *var, T value, MemoryPool *pool) {
  return var != 0 && var->setOwnedString(value, pool);
}
inline bool variantSetUnsignedInteger(VariantData *var, UInt value) {
  if (!var) return false;
  var->setUnsignedInteger(value);
  return true;
}
inline size_t variantSize(const VariantData *var) {
  return var != 0 ? var->size() : 0;
}
inline CollectionData *variantToArray(VariantData *var) {
  if (!var) return 0;
  return &var->toArray();
}
inline CollectionData *variantToObject(VariantData *var) {
  if (!var) return 0;
  return &var->toObject();
}
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
class ArrayRef;
class ObjectRef;
template <typename TData>
class VariantRefBase {
 public:
  template <typename T>
  FORCE_INLINE typename enable_if<is_integral<T>::value, bool>::type is()
      const {
    return variantIsInteger(_data);
  }
  template <typename T>
  FORCE_INLINE typename enable_if<is_floating_point<T>::value, bool>::type is()
      const {
    return variantIsFloat(_data);
  }
  template <typename T>
  FORCE_INLINE typename enable_if<is_same<T, bool>::value, bool>::type is()
      const {
    return variantIsBoolean(_data);
  }
  template <typename T>
  FORCE_INLINE typename enable_if<is_same<T, const char *>::value ||
                                      is_same<T, char *>::value ||
                                      IsWriteableString<T>::value,
                                  bool>::type
  is() const {
    return variantIsString(_data);
  }
  template <typename T>
  FORCE_INLINE typename enable_if<
      is_same<typename remove_const<T>::type, ArrayRef>::value, bool>::type
  is() const {
    return variantIsArray(_data);
  }
  template <typename T>
  FORCE_INLINE typename enable_if<
      is_same<typename remove_const<T>::type, ObjectRef>::value, bool>::type
  is() const {
    return variantIsObject(_data);
  }
  FORCE_INLINE bool isNull() const {
    return variantIsNull(_data);
  }
  size_t size() const {
    return variantSize(_data);
  }
 protected:
  VariantRefBase(TData *data) : _data(data) {}
  TData *_data;
};
class VariantRef : public VariantRefBase<VariantData>,
                   public VariantOperators<VariantRef>,
                   public Visitable {
  typedef VariantRefBase<VariantData> base_type;
  friend class VariantConstRef;
 public:
  FORCE_INLINE VariantRef(MemoryPool *pool, VariantData *data)
      : base_type(data), _pool(pool) {}
  FORCE_INLINE VariantRef() : base_type(0), _pool(0) {}
  FORCE_INLINE bool set(bool value) const {
    return variantSetBoolean(_data, value);
  }
  template <typename T>
  FORCE_INLINE bool set(
      T value,
      typename enable_if<is_floating_point<T>::value>::type * = 0) const {
    return variantSetFloat(_data, static_cast<Float>(value));
  }
  template <typename T>
  FORCE_INLINE bool set(
      T value,
      typename enable_if<is_integral<T>::value && is_signed<T>::value>::type * =
          0) const {
    return variantSetSignedInteger(_data, value);
  }
  template <typename T>
  FORCE_INLINE bool set(
      T value, typename enable_if<is_integral<T>::value &&
                                  is_unsigned<T>::value>::type * = 0) const {
    return variantSetUnsignedInteger(_data, static_cast<UInt>(value));
  }
  FORCE_INLINE bool set(SerializedValue<const char *> value) const {
    return variantSetLinkedRaw(_data, value);
  }
  template <typename T>
  FORCE_INLINE bool set(
      SerializedValue<T> value,
      typename enable_if<!is_same<const char *, T>::value>::type * = 0) const {
    return variantSetOwnedRaw(_data, value, _pool);
  }
  template <typename T>
  FORCE_INLINE bool set(
      const T &value,
      typename enable_if<IsString<T>::value>::type * = 0) const {
    return variantSetOwnedString(_data, wrapString(value), _pool);
  }
  template <typename T>
  FORCE_INLINE bool set(
      T *value, typename enable_if<IsString<T *>::value>::type * = 0) const {
    return variantSetOwnedString(_data, wrapString(value), _pool);
  }
  FORCE_INLINE bool set(const char *value) const {
    return variantSetLinkedString(_data, value);
  }
  bool set(VariantConstRef value) const;
  bool set(VariantRef value) const;
  FORCE_INLINE bool set(ArrayRef array) const;
  FORCE_INLINE bool set(const ArraySubscript &) const;
  FORCE_INLINE bool set(ObjectRef object) const;
  template <typename TString>
  FORCE_INLINE bool set(const ObjectSubscript<TString> &) const;
  template <typename T>
  FORCE_INLINE typename enable_if<!is_same<T, ArrayRef>::value &&
                                      !is_same<T, ObjectRef>::value &&
                                      !is_same<T, VariantRef>::value,
                                  typename VariantAs<T>::type>::type
  as() const {
    return variantAs<T>(_data);
  }
  template <typename T>
  FORCE_INLINE typename enable_if<is_same<T, ArrayRef>::value, T>::type as()
      const;
  template <typename T>
  FORCE_INLINE typename enable_if<is_same<T, ObjectRef>::value, T>::type as()
      const;
  template <typename T>
  FORCE_INLINE typename enable_if<is_same<T, VariantRef>::value, T>::type as()
      const {
    return *this;
  }
  template <typename Visitor>
  void accept(Visitor &visitor) const {
    variantAccept(_data, visitor);
  }
  FORCE_INLINE bool operator==(VariantRef lhs) const {
    return variantEquals(_data, lhs._data);
  }
  FORCE_INLINE bool operator!=(VariantRef lhs) const {
    return !variantEquals(_data, lhs._data);
  }
  template <typename T>
  typename enable_if<is_same<T, ArrayRef>::value, ArrayRef>::type to() const;
  template <typename T>
  typename enable_if<is_same<T, ObjectRef>::value, ObjectRef>::type to() const;
  template <typename T>
  typename enable_if<is_same<T, VariantRef>::value, VariantRef>::type to()
      const;
 private:
  MemoryPool *_pool;
};
class VariantConstRef : public VariantRefBase<const VariantData>,
                        public VariantOperators<VariantConstRef>,
                        public Visitable {
  typedef VariantRefBase<const VariantData> base_type;
  friend class VariantRef;
 public:
  VariantConstRef() : base_type(0) {}
  VariantConstRef(const VariantData *data) : base_type(data) {}
  VariantConstRef(VariantRef var) : base_type(var._data) {}
  template <typename Visitor>
  void accept(Visitor &visitor) const {
    variantAccept(_data, visitor);
  }
  template <typename T>
  FORCE_INLINE typename VariantConstAs<T>::type as() const {
    return variantAs<typename VariantConstAs<T>::type>(_data);
  }
  FORCE_INLINE VariantConstRef operator[](size_t index) const;
  template <typename TString>
  FORCE_INLINE
      typename enable_if<IsString<TString>::value, VariantConstRef>::type
      operator[](const TString &key) const {
    return VariantConstRef(objectGet(variantAsObject(_data), wrapString(key)));
  }
  template <typename TString>
  FORCE_INLINE
      typename enable_if<IsString<TString *>::value, VariantConstRef>::type
      operator[](TString *key) const {
    const CollectionData *obj = variantAsObject(_data);
    return VariantConstRef(obj ? obj->get(wrapString(key)) : 0);
  }
};
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
class VariantPtr {
 public:
  VariantPtr(MemoryPool *pool, VariantData *data) : _variant(pool, data) {}
  VariantRef *operator->() {
    return &_variant;
  }
  VariantRef &operator*() {
    return _variant;
  }
 private:
  VariantRef _variant;
};
class ArrayIterator {
 public:
  ArrayIterator() : _slot(0) {}
  explicit ArrayIterator(MemoryPool *pool, VariantSlot *slot)
      : _pool(pool), _slot(slot) {}
  VariantRef operator*() const {
    return VariantRef(_pool, _slot->data());
  }
  VariantPtr operator->() {
    return VariantPtr(_pool, _slot->data());
  }
  bool operator==(const ArrayIterator &other) const {
    return _slot == other._slot;
  }
  bool operator!=(const ArrayIterator &other) const {
    return _slot != other._slot;
  }
  ArrayIterator &operator++() {
    _slot = _slot->next();
    return *this;
  }
  ArrayIterator &operator+=(size_t distance) {
    _slot = _slot->next(distance);
    return *this;
  }
  VariantSlot *internal() {
    return _slot;
  }
 private:
  MemoryPool *_pool;
  VariantSlot *_slot;
};
class VariantConstPtr {
 public:
  VariantConstPtr(const VariantData *data) : _variant(data) {}
  VariantConstRef *operator->() {
    return &_variant;
  }
  VariantConstRef &operator*() {
    return _variant;
  }
 private:
  VariantConstRef _variant;
};
class ArrayConstRefIterator {
 public:
  ArrayConstRefIterator() : _slot(0) {}
  explicit ArrayConstRefIterator(const VariantSlot *slot) : _slot(slot) {}
  VariantConstRef operator*() const {
    return VariantConstRef(_slot->data());
  }
  VariantConstPtr operator->() {
    return VariantConstPtr(_slot->data());
  }
  bool operator==(const ArrayConstRefIterator &other) const {
    return _slot == other._slot;
  }
  bool operator!=(const ArrayConstRefIterator &other) const {
    return _slot != other._slot;
  }
  ArrayConstRefIterator &operator++() {
    _slot = _slot->next();
    return *this;
  }
  ArrayConstRefIterator &operator+=(size_t distance) {
    _slot = _slot->next(distance);
    return *this;
  }
  const VariantSlot *internal() {
    return _slot;
  }
 private:
  const VariantSlot *_slot;
};
}  // namespace ARDUINOJSON_NAMESPACE
#define JSON_ARRAY_SIZE(NUMBER_OF_ELEMENTS) \
  ((NUMBER_OF_ELEMENTS) * sizeof(ARDUINOJSON_NAMESPACE::VariantSlot))
namespace ARDUINOJSON_NAMESPACE {
class ObjectRef;
class ArraySubscript;
template <typename TData>
class ArrayRefBase {
 public:
  template <typename Visitor>
  FORCE_INLINE void accept(Visitor& visitor) const {
    arrayAccept(_data, visitor);
  }
  FORCE_INLINE bool isNull() const {
    return _data == 0;
  }
  FORCE_INLINE VariantConstRef operator[](size_t index) const {
    return VariantConstRef(_data ? _data->get(index) : 0);
  }
  FORCE_INLINE size_t size() const {
    return _data ? _data->size() : 0;
  }
 protected:
  ArrayRefBase(TData* data) : _data(data) {}
  TData* _data;
};
class ArrayConstRef : public ArrayRefBase<const CollectionData>,
                      public Visitable {
  friend class ArrayRef;
  typedef ArrayRefBase<const CollectionData> base_type;
 public:
  typedef ArrayConstRefIterator iterator;
  FORCE_INLINE iterator begin() const {
    if (!_data) return iterator();
    return iterator(_data->head());
  }
  FORCE_INLINE iterator end() const {
    return iterator();
  }
  FORCE_INLINE ArrayConstRef() : base_type(0) {}
  FORCE_INLINE ArrayConstRef(const CollectionData* data) : base_type(data) {}
  FORCE_INLINE bool operator==(ArrayConstRef rhs) const {
    return arrayEquals(_data, rhs._data);
  }
};
class ArrayRef : public ArrayRefBase<CollectionData>, public Visitable {
  typedef ArrayRefBase<CollectionData> base_type;
 public:
  typedef ArrayIterator iterator;
  FORCE_INLINE ArrayRef() : base_type(0), _pool(0) {}
  FORCE_INLINE ArrayRef(MemoryPool* pool, CollectionData* data)
      : base_type(data), _pool(pool) {}
  operator VariantRef() {
    return VariantRef(_pool, reinterpret_cast<VariantData*>(_data));
  }
  operator ArrayConstRef() const {
    return ArrayConstRef(_data);
  }
  template <typename T>
  FORCE_INLINE bool add(const T& value) const {
    return add().set(value);
  }
  FORCE_INLINE bool add(ArrayRef value) const {
    return add().set(value);
  }
  template <typename T>
  FORCE_INLINE bool add(T* value) const {
    return add().set(value);
  }
  VariantRef add() const {
    return VariantRef(_pool, arrayAdd(_data, _pool));
  }
  FORCE_INLINE iterator begin() const {
    if (!_data) return iterator();
    return iterator(_pool, _data->head());
  }
  FORCE_INLINE iterator end() const {
    return iterator();
  }
  template <typename T, size_t N>
  FORCE_INLINE bool copyFrom(T (&array)[N]) const {
    return copyFrom(array, N);
  }
  template <typename T>
  bool copyFrom(T* array, size_t len) const {
    bool ok = true;
    for (size_t i = 0; i < len; i++) {
      ok &= add(array[i]);
    }
    return ok;
  }
  template <typename T, size_t N1, size_t N2>
  bool copyFrom(T (&array)[N1][N2]) const {
    bool ok = true;
    for (size_t i = 0; i < N1; i++) {
      ArrayRef nestedArray = createNestedArray();
      for (size_t j = 0; j < N2; j++) {
        ok &= nestedArray.add(array[i][j]);
      }
    }
    return ok;
  }
  FORCE_INLINE bool copyFrom(ArrayRef src) const {
    if (!_data || !src._data) return false;
    return _data->copyFrom(*src._data, _pool);
  }
  template <typename T, size_t N>
  FORCE_INLINE size_t copyTo(T (&array)[N]) const {
    return copyTo(array, N);
  }
  template <typename T>
  size_t copyTo(T* array, size_t len) const {
    size_t i = 0;
    for (iterator it = begin(); it != end() && i < len; ++it) array[i++] = *it;
    return i;
  }
  template <typename T, size_t N1, size_t N2>
  void copyTo(T (&array)[N1][N2]) const {
    if (!_data) return;
    size_t i = 0;
    for (iterator it = begin(); it != end() && i < N1; ++it) {
      it->as<ArrayRef>().copyTo(array[i++]);
    }
  }
  FORCE_INLINE ArrayRef createNestedArray() const;
  FORCE_INLINE ObjectRef createNestedObject() const;
  FORCE_INLINE ArraySubscript operator[](size_t index) const;
  FORCE_INLINE bool operator==(ArrayRef rhs) const {
    return arrayEquals(_data, rhs._data);
  }
  FORCE_INLINE VariantRef get(size_t index) const {
    return VariantRef(_pool, _data ? _data->get(index) : 0);
  }
  FORCE_INLINE void remove(iterator it) const {
    if (!_data) return;
    _data->remove(it.internal());
  }
  FORCE_INLINE void remove(size_t index) const {
    if (!_data) return;
    _data->remove(index);
  }
 private:
  MemoryPool* _pool;
};
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
template <typename Visitor>
void objectAccept(const CollectionData *obj, Visitor &visitor) {
  if (obj)
    visitor.visitObject(*obj);
  else
    visitor.visitNull();
}
template <typename TKey>
inline bool objectContainsKey(const CollectionData *obj, TKey key) {
  return obj && obj->containsKey(key);
}
inline bool objectEquals(const CollectionData *lhs, const CollectionData *rhs) {
  if (lhs == rhs) return true;
  if (!lhs || !rhs) return false;
  return lhs->equalsObject(*rhs);
}
template <typename TKey>
inline VariantData *objectGet(const CollectionData *obj, TKey key) {
  if (!obj) return 0;
  return obj->get(key);
}
template <typename TKey>
void objectRemove(CollectionData *obj, TKey key) {
  if (!obj) return;
  obj->remove(key);
}
template <typename TKey>
inline VariantData *objectSet(CollectionData *obj, TKey key, MemoryPool *pool) {
  if (!obj) return 0;
  if (key.isNull()) return 0;
  VariantData *var = obj->get(key);
  if (var) return var;
  return obj->add(key, pool);
}
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
class String {
 public:
  String() : _data(0) {}
  String(const char* slot) : _data(slot) {}
  const char* c_str() const {
    return _data;
  }
  bool isNull() const {
    return !_data;
  }
  friend bool operator==(String lhs, String rhs) {
    if (lhs._data == rhs._data) return true;
    if (!lhs._data) return false;
    if (!rhs._data) return false;
    return strcmp(lhs._data, rhs._data) == 0;
  }
 private:
  const char* _data;
};
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
class Pair {
 public:
  Pair(MemoryPool* pool, VariantSlot* slot) {
    if (slot) {
      _key = slot->key();
      _value = VariantRef(pool, slot->data());
    }
  }
  String key() const {
    return _key;
  }
  VariantRef value() const {
    return _value;
  }
 private:
  String _key;
  VariantRef _value;
};
class PairConst {
 public:
  PairConst(const VariantSlot* slot) {
    if (slot) {
      _key = slot->key();
      _value = VariantConstRef(slot->data());
    }
  }
  String key() const {
    return _key;
  }
  VariantConstRef value() const {
    return _value;
  }
 private:
  String _key;
  VariantConstRef _value;
};
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
class PairPtr {
 public:
  PairPtr(MemoryPool *pool, VariantSlot *slot) : _pair(pool, slot) {}
  const Pair *operator->() const {
    return &_pair;
  }
  const Pair &operator*() const {
    return _pair;
  }
 private:
  Pair _pair;
};
class ObjectIterator {
 public:
  ObjectIterator() : _slot(0) {}
  explicit ObjectIterator(MemoryPool *pool, VariantSlot *slot)
      : _pool(pool), _slot(slot) {}
  Pair operator*() const {
    return Pair(_pool, _slot);
  }
  PairPtr operator->() {
    return PairPtr(_pool, _slot);
  }
  bool operator==(const ObjectIterator &other) const {
    return _slot == other._slot;
  }
  bool operator!=(const ObjectIterator &other) const {
    return _slot != other._slot;
  }
  ObjectIterator &operator++() {
    _slot = _slot->next();
    return *this;
  }
  ObjectIterator &operator+=(size_t distance) {
    _slot = _slot->next(distance);
    return *this;
  }
  VariantSlot *internal() {
    return _slot;
  }
 private:
  MemoryPool *_pool;
  VariantSlot *_slot;
};
class PairConstPtr {
 public:
  PairConstPtr(const VariantSlot *slot) : _pair(slot) {}
  const PairConst *operator->() const {
    return &_pair;
  }
  const PairConst &operator*() const {
    return _pair;
  }
 private:
  PairConst _pair;
};
class ObjectConstIterator {
 public:
  ObjectConstIterator() : _slot(0) {}
  explicit ObjectConstIterator(const VariantSlot *slot) : _slot(slot) {}
  PairConst operator*() const {
    return PairConst(_slot);
  }
  PairConstPtr operator->() {
    return PairConstPtr(_slot);
  }
  bool operator==(const ObjectConstIterator &other) const {
    return _slot == other._slot;
  }
  bool operator!=(const ObjectConstIterator &other) const {
    return _slot != other._slot;
  }
  ObjectConstIterator &operator++() {
    _slot = _slot->next();
    return *this;
  }
  ObjectConstIterator &operator+=(size_t distance) {
    _slot = _slot->next(distance);
    return *this;
  }
  const VariantSlot *internal() {
    return _slot;
  }
 private:
  const VariantSlot *_slot;
};
}  // namespace ARDUINOJSON_NAMESPACE
#define JSON_OBJECT_SIZE(NUMBER_OF_ELEMENTS) \
  ((NUMBER_OF_ELEMENTS) * sizeof(ARDUINOJSON_NAMESPACE::VariantSlot))
namespace ARDUINOJSON_NAMESPACE {
template <typename TData>
class ObjectRefBase {
 public:
  template <typename Visitor>
  FORCE_INLINE void accept(Visitor& visitor) const {
    objectAccept(_data, visitor);
  }
  template <typename TKey>
  FORCE_INLINE bool containsKey(const TKey& key) const {
    return objectContainsKey(_data, wrapString(key));
  }
  template <typename TKey>
  FORCE_INLINE bool containsKey(TKey* key) const {
    return objectContainsKey(_data, wrapString(key));
  }
  FORCE_INLINE bool isNull() const {
    return _data == 0;
  }
  FORCE_INLINE size_t size() const {
    return _data ? _data->size() : 0;
  }
 protected:
  ObjectRefBase(TData* data) : _data(data) {}
  TData* _data;
};
class ObjectConstRef : public ObjectRefBase<const CollectionData>,
                       public Visitable {
  friend class ObjectRef;
  typedef ObjectRefBase<const CollectionData> base_type;
 public:
  typedef ObjectConstIterator iterator;
  ObjectConstRef() : base_type(0) {}
  ObjectConstRef(const CollectionData* data) : base_type(data) {}
  FORCE_INLINE iterator begin() const {
    if (!_data) return iterator();
    return iterator(_data->head());
  }
  FORCE_INLINE iterator end() const {
    return iterator();
  }
  template <typename TKey>
  FORCE_INLINE VariantConstRef get(const TKey& key) const {
    return get_impl(wrapString(key));
  }
  template <typename TKey>
  FORCE_INLINE VariantConstRef get(TKey* key) const {
    return get_impl(wrapString(key));
  }
  template <typename TKey>
  FORCE_INLINE typename enable_if<IsString<TKey>::value, VariantConstRef>::type
  operator[](const TKey& key) const {
    return get_impl(wrapString(key));
  }
  template <typename TKey>
  FORCE_INLINE typename enable_if<IsString<TKey*>::value, VariantConstRef>::type
  operator[](TKey* key) const {
    return get_impl(wrapString(key));
  }
  FORCE_INLINE bool operator==(ObjectConstRef rhs) const {
    return objectEquals(_data, rhs._data);
  }
 private:
  template <typename TKey>
  FORCE_INLINE VariantConstRef get_impl(TKey key) const {
    return VariantConstRef(objectGet(_data, key));
  }
};
class ObjectRef : public ObjectRefBase<CollectionData>, public Visitable {
  typedef ObjectRefBase<CollectionData> base_type;
 public:
  typedef ObjectIterator iterator;
  FORCE_INLINE ObjectRef() : base_type(0), _pool(0) {}
  FORCE_INLINE ObjectRef(MemoryPool* buf, CollectionData* data)
      : base_type(data), _pool(buf) {}
  operator VariantRef() const {
    return VariantRef(_pool, reinterpret_cast<VariantData*>(_data));
  }
  operator ObjectConstRef() const {
    return ObjectConstRef(_data);
  }
  FORCE_INLINE iterator begin() const {
    if (!_data) return iterator();
    return iterator(_pool, _data->head());
  }
  FORCE_INLINE iterator end() const {
    return iterator();
  }
  void clear() const {
    if (!_data) return;
    _data->clear();
  }
  FORCE_INLINE bool copyFrom(ObjectConstRef src) {
    if (!_data || !src._data) return false;
    return _data->copyFrom(*src._data, _pool);
  }
  template <typename TKey>
  FORCE_INLINE ArrayRef createNestedArray(const TKey& key) const;
  template <typename TKey>
  FORCE_INLINE ArrayRef createNestedArray(TKey* key) const;
  template <typename TKey>
  FORCE_INLINE ObjectRef createNestedObject(const TKey& key) const {
    return set(key).template to<ObjectRef>();
  }
  template <typename TKey>
  FORCE_INLINE ObjectRef createNestedObject(TKey* key) const {
    return set(key).template to<ObjectRef>();
  }
  template <typename TKey>
  FORCE_INLINE VariantRef get(const TKey& key) const {
    return get_impl(wrapString(key));
  }
  template <typename TKey>
  FORCE_INLINE VariantRef get(TKey* key) const {
    return get_impl(wrapString(key));
  }
  template <typename TKey>
  FORCE_INLINE ObjectSubscript<const TKey&> operator[](const TKey& key) const {
    return ObjectSubscript<const TKey&>(*this, key);
  }
  template <typename TKey>
  FORCE_INLINE ObjectSubscript<TKey*> operator[](TKey* key) const {
    return ObjectSubscript<TKey*>(*this, key);
  }
  FORCE_INLINE bool operator==(ObjectRef rhs) const {
    return objectEquals(_data, rhs._data);
  }
  FORCE_INLINE void remove(iterator it) const {
    if (!_data) return;
    _data->remove(it.internal());
  }
  template <typename TKey>
  FORCE_INLINE void remove(const TKey& key) const {
    objectRemove(_data, wrapString(key));
  }
  template <typename TKey>
  FORCE_INLINE void remove(TKey* key) const {
    objectRemove(_data, wrapString(key));
  }
  template <typename TKey>
  FORCE_INLINE VariantRef set(TKey* key) const {
    return set_impl(wrapString(key));
  }
  template <typename TKey>
  FORCE_INLINE VariantRef set(const TKey& key) const {
    return set_impl(wrapString(key));
  }
 private:
  template <typename TKey>
  FORCE_INLINE VariantRef get_impl(TKey key) const {
    return VariantRef(_pool, objectGet(_data, key));
  }
  template <typename TKey>
  FORCE_INLINE VariantRef set_impl(TKey key) const {
    return VariantRef(_pool, objectSet(_data, key, _pool));
  }
  MemoryPool* _pool;
};
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
class ArrayRef;
class ObjectRef;
class VariantRef;
template <typename T>
struct VariantTo {};
template <>
struct VariantTo<ArrayRef> {
  typedef ArrayRef type;
};
template <>
struct VariantTo<ObjectRef> {
  typedef ObjectRef type;
};
template <>
struct VariantTo<VariantRef> {
  typedef VariantRef type;
};
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
class JsonDocument : public Visitable {
 public:
  uint8_t nestingLimit;
  template <typename Visitor>
  void accept(Visitor& visitor) const {
    return getVariant().accept(visitor);
  }
  template <typename T>
  typename VariantAs<T>::type as() {
    return getVariant().template as<T>();
  }
  template <typename T>
  typename VariantConstAs<T>::type as() const {
    return getVariant().template as<T>();
  }
  void clear() {
    _pool.clear();
    _data.setNull();
  }
  template <typename T>
  bool is() const {
    return getVariant().template is<T>();
  }
  size_t memoryUsage() const {
    return _pool.size();
  }
  size_t capacity() const {
    return _pool.capacity();
  }
  template <typename T>
  typename VariantTo<T>::type to() {
    clear();
    return getVariant().template to<T>();
  }
  MemoryPool& memoryPool() {
    return _pool;
  }
  VariantData& data() {
    return _data;
  }
 protected:
  JsonDocument(MemoryPool pool)
      : nestingLimit(ARDUINOJSON_DEFAULT_NESTING_LIMIT), _pool(pool) {}
  JsonDocument(char* buf, size_t capa)
      : nestingLimit(ARDUINOJSON_DEFAULT_NESTING_LIMIT), _pool(buf, capa) {}
  void copy(const JsonDocument& src) {
    nestingLimit = src.nestingLimit;
    to<VariantRef>().set(src.as<VariantRef>());
  }
  void replacePool(MemoryPool pool) {
    _pool = pool;
  }
 private:
  VariantRef getVariant() {
    return VariantRef(&_pool, &_data);
  }
  VariantConstRef getVariant() const {
    return VariantConstRef(&_data);
  }
  MemoryPool _pool;
  VariantData _data;
};
}  // namespace ARDUINOJSON_NAMESPACE
#include <stdlib.h>  // malloc, free
namespace ARDUINOJSON_NAMESPACE {
class DynamicJsonDocument : public JsonDocument {
 public:
  DynamicJsonDocument(size_t capa = ARDUINOJSON_DEFAULT_POOL_SIZE)
      : JsonDocument(allocPool(addPadding(capa))) {}
  DynamicJsonDocument(const DynamicJsonDocument& src)
      : JsonDocument(allocPool(src.capacity())) {
    copy(src);
  }
  DynamicJsonDocument(const JsonDocument& src)
      : JsonDocument(allocPool(src.capacity())) {
    copy(src);
  }
  ~DynamicJsonDocument() {
    freePool();
  }
  DynamicJsonDocument& operator=(const DynamicJsonDocument& src) {
    reallocPoolIfTooSmall(src.memoryUsage());
    copy(src);
    return *this;
  }
  template <typename T>
  DynamicJsonDocument& operator=(const JsonDocument& src) {
    reallocPoolIfTooSmall(src.memoryUsage());
    copy(src);
    return *this;
  }
 private:
  MemoryPool allocPool(size_t capa) {
    return MemoryPool(reinterpret_cast<char*>(malloc(capa)), capa);
  }
  void reallocPoolIfTooSmall(size_t requiredSize) {
    if (requiredSize <= capacity()) return;
    freePool();
    replacePool(allocPool(addPadding(requiredSize)));
  }
  void freePool() {
    free(memoryPool().buffer());
  }
};
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
template <size_t CAPACITY>
class StaticJsonDocument : public JsonDocument {
  static const size_t ACTUAL_CAPACITY =
      AddPadding<Max<1, CAPACITY>::value>::value;
 public:
  StaticJsonDocument() : JsonDocument(_buffer, ACTUAL_CAPACITY) {}
  StaticJsonDocument(const JsonDocument& src)
      : JsonDocument(_buffer, ACTUAL_CAPACITY) {
    copy(src);
  }
  StaticJsonDocument operator=(const JsonDocument& src) {
    copy(src);
    return *this;
  }
 private:
  char _buffer[ACTUAL_CAPACITY];
};
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
inline ArrayRef ArrayRef::createNestedArray() const {
  return add().to<ArrayRef>();
}
inline ObjectRef ArrayRef::createNestedObject() const {
  return add().to<ObjectRef>();
}
}  // namespace ARDUINOJSON_NAMESPACE
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4522)
#endif
namespace ARDUINOJSON_NAMESPACE {
class ArraySubscript : public VariantOperators<ArraySubscript>,
                       public Visitable {
 public:
  FORCE_INLINE ArraySubscript(ArrayRef array, size_t index)
      : _array(array), _index(index) {}
  FORCE_INLINE ArraySubscript& operator=(const ArraySubscript& src) {
    get_impl().set(src.as<VariantConstRef>());
    return *this;
  }
  template <typename T>
  FORCE_INLINE ArraySubscript& operator=(const T& src) {
    get_impl().set(src);
    return *this;
  }
  template <typename T>
  FORCE_INLINE ArraySubscript& operator=(T* src) {
    get_impl().set(src);
    return *this;
  }
  FORCE_INLINE bool isNull() const {
    return get_impl().isNull();
  }
  template <typename T>
  FORCE_INLINE typename VariantAs<T>::type as() const {
    return get_impl().as<T>();
  }
  template <typename T>
  FORCE_INLINE bool is() const {
    return get_impl().is<T>();
  }
  template <typename T>
  FORCE_INLINE typename VariantTo<T>::type to() const {
    return get_impl().to<T>();
  }
  template <typename TValue>
  FORCE_INLINE bool set(const TValue& value) const {
    return get_impl().set(value);
  }
  template <typename TValue>
  FORCE_INLINE bool set(TValue* value) const {
    return get_impl().set(value);
  }
  template <typename Visitor>
  void accept(Visitor& visitor) const {
    return get_impl().accept(visitor);
  }
  FORCE_INLINE size_t size() const {
    return get_impl().size();
  }
 private:
  FORCE_INLINE VariantRef get_impl() const {
    return _array.get(_index);
  }
  ArrayRef _array;
  const size_t _index;
};
template <typename TImpl>
inline ArraySubscript VariantSubscripts<TImpl>::operator[](size_t index) const {
  return impl()->template as<ArrayRef>()[index];
}
inline ArraySubscript ArrayRef::operator[](size_t index) const {
  return ArraySubscript(*this, index);
}
}  // namespace ARDUINOJSON_NAMESPACE
#ifdef _MSC_VER
#pragma warning(pop)
#endif
namespace ARDUINOJSON_NAMESPACE {
inline VariantSlot* CollectionData::addSlot(MemoryPool* pool) {
  VariantSlot* slot = pool->allocVariant();
  if (!slot) return 0;
  if (_tail) {
    _tail->setNextNotNull(slot);
    _tail = slot;
  } else {
    _head = slot;
    _tail = slot;
  }
  slot->clear();
  return slot;
}
inline VariantData* CollectionData::add(MemoryPool* pool) {
  return addSlot(pool)->data();
}
template <typename TKey>
inline VariantData* CollectionData::add(TKey key, MemoryPool* pool) {
  VariantSlot* slot = addSlot(pool);
  if (!slotSetKey(slot, key, pool)) return 0;
  return slot->data();
}
inline void CollectionData::clear() {
  _head = 0;
  _tail = 0;
}
template <typename TKey>
inline bool CollectionData::containsKey(const TKey& key) const {
  return getSlot(key) != 0;
}
inline bool CollectionData::copyFrom(const CollectionData& src,
                                     MemoryPool* pool) {
  clear();
  for (VariantSlot* s = src._head; s; s = s->next()) {
    VariantData* var;
    if (s->key() != 0) {
      if (s->ownsKey())
        var = add(RamStringWrapper(s->key()), pool);
      else
        var = add(ConstRamStringWrapper(s->key()), pool);
    } else {
      var = add(pool);
    }
    if (!var) return false;
    if (!var->copyFrom(*s->data(), pool)) return false;
  }
  return true;
}
inline bool CollectionData::equalsObject(const CollectionData& other) const {
  size_t count = 0;
  for (VariantSlot* slot = _head; slot; slot = slot->next()) {
    VariantData* v1 = slot->data();
    VariantData* v2 = other.get(wrapString(slot->key()));
    if (!variantEquals(v1, v2)) return false;
    count++;
  }
  return count == other.size();
}
inline bool CollectionData::equalsArray(const CollectionData& other) const {
  VariantSlot* s1 = _head;
  VariantSlot* s2 = other._head;
  for (;;) {
    if (s1 == s2) return true;
    if (!s1 || !s2) return false;
    if (!variantEquals(s1->data(), s2->data())) return false;
    s1 = s1->next();
    s2 = s2->next();
  }
}
template <typename TKey>
inline VariantSlot* CollectionData::getSlot(TKey key) const {
  VariantSlot* slot = _head;
  while (slot) {
    if (key.equals(slot->key())) break;
    slot = slot->next();
  }
  return slot;
}
inline VariantSlot* CollectionData::getSlot(size_t index) const {
  return _head->next(index);
}
inline VariantSlot* CollectionData::getPreviousSlot(VariantSlot* target) const {
  VariantSlot* current = _head;
  while (current) {
    VariantSlot* next = current->next();
    if (next == target) return current;
    current = next;
  }
  return 0;
}
template <typename TKey>
inline VariantData* CollectionData::get(TKey key) const {
  VariantSlot* slot = getSlot(key);
  return slot ? slot->data() : 0;
}
inline VariantData* CollectionData::get(size_t index) const {
  VariantSlot* slot = getSlot(index);
  return slot ? slot->data() : 0;
}
inline void CollectionData::remove(VariantSlot* slot) {
  if (!slot) return;
  VariantSlot* prev = getPreviousSlot(slot);
  VariantSlot* next = slot->next();
  if (prev)
    prev->setNext(next);
  else
    _head = next;
  if (!next) _tail = prev;
}
inline void CollectionData::remove(size_t index) {
  remove(getSlot(index));
}
inline size_t CollectionData::size() const {
  return slotSize(_head);
}
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
template <typename TString>
inline ArrayRef ObjectRef::createNestedArray(const TString& key) const {
  return set(key).template to<ArrayRef>();
}
template <typename TString>
inline ArrayRef ObjectRef::createNestedArray(TString* key) const {
  return set(key).template to<ArrayRef>();
}
}  // namespace ARDUINOJSON_NAMESPACE
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4522)
#endif
namespace ARDUINOJSON_NAMESPACE {
template <typename TStringRef>
class ObjectSubscript : public VariantOperators<ObjectSubscript<TStringRef> >,
                        public Visitable {
  typedef ObjectSubscript<TStringRef> this_type;
 public:
  FORCE_INLINE ObjectSubscript(ObjectRef object, TStringRef key)
      : _object(object), _key(key) {}
  operator VariantConstRef() const {
    return get_impl();
  }
  FORCE_INLINE this_type &operator=(const this_type &src) {
    set_impl().set(src);
    return *this;
  }
  template <typename TValue>
  FORCE_INLINE typename enable_if<!is_array<TValue>::value, this_type &>::type
  operator=(const TValue &src) {
    set_impl().set(src);
    return *this;
  }
  template <typename TValue>
  FORCE_INLINE this_type &operator=(TValue *src) {
    set_impl().set(src);
    return *this;
  }
  FORCE_INLINE bool isNull() const {
    return get_impl().isNull();
  }
  template <typename TValue>
  FORCE_INLINE typename VariantAs<TValue>::type as() const {
    return get_impl().template as<TValue>();
  }
  template <typename TValue>
  FORCE_INLINE bool is() const {
    return get_impl().template is<TValue>();
  }
  template <typename TValue>
  FORCE_INLINE typename VariantTo<TValue>::type to() {
    return set_impl().template to<TValue>();
  }
  template <typename TValue>
  FORCE_INLINE typename enable_if<!is_array<TValue>::value, bool>::type set(
      const TValue &value) {
    return set_impl().set(value);
  }
  template <typename TValue>
  FORCE_INLINE bool set(const TValue *value) {
    return set_impl().set(value);
  }
  template <typename Visitor>
  void accept(Visitor &visitor) const {
    return get_impl().accept(visitor);
  }
 private:
  FORCE_INLINE VariantRef get_impl() const {
    return _object.get(_key);
  }
  FORCE_INLINE VariantRef set_impl() const {
    return _object.set(_key);
  }
  ObjectRef _object;
  TStringRef _key;
};
template <typename TImpl>
template <typename TString>
inline typename enable_if<IsString<TString>::value,
                          ObjectSubscript<const TString &> >::type
    VariantSubscripts<TImpl>::operator[](const TString &key) const {
  return impl()->template as<ObjectRef>()[key];
}
template <typename TImpl>
template <typename TString>
inline typename enable_if<IsString<TString *>::value,
                          ObjectSubscript<TString *> >::type
    VariantSubscripts<TImpl>::operator[](TString *key) const {
  return impl()->template as<ObjectRef>()[key];
}
}  // namespace ARDUINOJSON_NAMESPACE
#ifdef _MSC_VER
#pragma warning(pop)
#endif
namespace ARDUINOJSON_NAMESPACE {
template <typename T>
inline typename enable_if<is_same<VariantConstRef, T>::value, T>::type
variantAs(const VariantData* _data) {
  return VariantConstRef(_data);
}
template <typename T>
inline typename enable_if<IsWriteableString<T>::value, T>::type variantAs(
    const VariantData* _data) {
  const char* cstr = _data != 0 ? _data->asString() : 0;
  if (cstr) return T(cstr);
  T s;
  serializeJson(VariantConstRef(_data), s);
  return s;
}
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
template <typename T>
inline T VariantData::asIntegral() const {
  switch (type()) {
    case VALUE_IS_POSITIVE_INTEGER:
    case VALUE_IS_BOOLEAN:
      return T(_content.asInteger);
    case VALUE_IS_NEGATIVE_INTEGER:
      return T(~_content.asInteger + 1);
    case VALUE_IS_LINKED_STRING:
    case VALUE_IS_OWNED_STRING:
      return parseInteger<T>(_content.asString);
    case VALUE_IS_FLOAT:
      return T(_content.asFloat);
    default:
      return 0;
  }
}
template <typename T>
inline T VariantData::asFloat() const {
  switch (type()) {
    case VALUE_IS_POSITIVE_INTEGER:
    case VALUE_IS_BOOLEAN:
      return static_cast<T>(_content.asInteger);
    case VALUE_IS_NEGATIVE_INTEGER:
      return -static_cast<T>(_content.asInteger);
    case VALUE_IS_LINKED_STRING:
    case VALUE_IS_OWNED_STRING:
      return parseFloat<T>(_content.asString);
    case VALUE_IS_FLOAT:
      return static_cast<T>(_content.asFloat);
    default:
      return 0;
  }
}
inline const char* VariantData::asString() const {
  switch (type()) {
    case VALUE_IS_LINKED_STRING:
    case VALUE_IS_OWNED_STRING:
      return _content.asString;
    default:
      return 0;
  }
}
inline bool VariantRef::set(ArrayRef array) const {
  return to<ArrayRef>().copyFrom(array);
}
inline bool VariantRef::set(const ArraySubscript& value) const {
  return set(value.as<VariantRef>());
}
inline bool VariantRef::set(ObjectRef object) const {
  return to<ObjectRef>().copyFrom(object);
}
template <typename TString>
inline bool VariantRef::set(const ObjectSubscript<TString>& value) const {
  return set(value.template as<VariantRef>());
}
inline bool VariantRef::set(VariantConstRef value) const {
  return variantCopyFrom(_data, value._data, _pool);
}
inline bool VariantRef::set(VariantRef value) const {
  return variantCopyFrom(_data, value._data, _pool);
}
template <typename T>
inline typename enable_if<is_same<T, ArrayRef>::value, T>::type VariantRef::as()
    const {
  return ArrayRef(_pool, _data != 0 ? _data->asArray() : 0);
}
template <typename T>
inline typename enable_if<is_same<T, ObjectRef>::value, T>::type
VariantRef::as() const {
  return ObjectRef(_pool, variantAsObject(_data));
}
template <typename T>
inline typename enable_if<is_same<T, ArrayRef>::value, ArrayRef>::type
VariantRef::to() const {
  return ArrayRef(_pool, variantToArray(_data));
}
template <typename T>
typename enable_if<is_same<T, ObjectRef>::value, ObjectRef>::type
VariantRef::to() const {
  return ObjectRef(_pool, variantToObject(_data));
}
template <typename T>
typename enable_if<is_same<T, VariantRef>::value, VariantRef>::type
VariantRef::to() const {
  variantSetNull(_data);
  return *this;
}
inline VariantConstRef VariantConstRef::operator[](size_t index) const {
  return ArrayConstRef(_data != 0 ? _data->asArray() : 0)[index];
}
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
class StringBuilder {
 public:
  explicit StringBuilder(MemoryPool* parent) : _parent(parent), _size(0) {
    _slot = _parent->allocExpandableString();
  }
  void append(const char* s) {
    while (*s) append(*s++);
  }
  void append(const char* s, size_t n) {
    while (n-- > 0) append(*s++);
  }
  void append(char c) {
    if (!_slot.value) return;
    if (_size >= _slot.size) {
      _slot.value = 0;
      return;
    }
    _slot.value[_size++] = c;
  }
  char* complete() {
    append('\0');
    if (_slot.value) {
      _parent->freezeString(_slot, _size);
    }
    return _slot.value;
  }
 private:
  MemoryPool* _parent;
  size_t _size;
  StringSlot _slot;
};
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
class StringCopier {
 public:
  typedef ARDUINOJSON_NAMESPACE::StringBuilder StringBuilder;
  StringCopier(MemoryPool* pool) : _pool(pool) {}
  StringBuilder startString() {
    return StringBuilder(_pool);
  }
 private:
  MemoryPool* _pool;
};
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
class StringMover {
 public:
  class StringBuilder {
   public:
    StringBuilder(char** ptr) : _writePtr(ptr), _startPtr(*ptr) {}
    void append(char c) {
      *(*_writePtr)++ = char(c);
    }
    char* complete() const {
      *(*_writePtr)++ = 0;
      return _startPtr;
    }
   private:
    char** _writePtr;
    char* _startPtr;
  };
  StringMover(char* ptr) : _ptr(ptr) {}
  StringBuilder startString() {
    return StringBuilder(&_ptr);
  }
 private:
  char* _ptr;
};
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
template <typename TInput, typename Enable = void>
struct StringStorage {
  typedef StringCopier type;
  static type create(MemoryPool& pool, TInput&) {
    return type(&pool);
  }
};
template <typename TChar>
struct StringStorage<TChar*,
                     typename enable_if<!is_const<TChar>::value>::type> {
  typedef StringMover type;
  static type create(MemoryPool&, TChar* input) {
    return type(reinterpret_cast<char*>(input));
  }
};
template <typename TInput>
typename StringStorage<TInput>::type makeStringStorage(MemoryPool& pool,
                                                       TInput& input) {
  return StringStorage<TInput>::create(pool, input);
}
template <typename TChar>
typename StringStorage<TChar*>::type makeStringStorage(MemoryPool& pool,
                                                       TChar* input) {
  return StringStorage<TChar*>::create(pool, input);
}
}  // namespace ARDUINOJSON_NAMESPACE
#if ARDUINOJSON_ENABLE_ARDUINO_STREAM
#include <Stream.h>
namespace ARDUINOJSON_NAMESPACE {
struct ArduinoStreamReader {
  Stream& _stream;
  char _current;
  bool _ended;
 public:
  explicit ArduinoStreamReader(Stream& stream)
      : _stream(stream), _current(0), _ended(false) {}
  char read() {
    char c = 0;
    _ended = _stream.readBytes(&c, 1) == 0;
    return c;
  }
  bool ended() const {
    return _ended;
  }
};
inline ArduinoStreamReader makeReader(Stream& input) {
  return ArduinoStreamReader(input);
}
}  // namespace ARDUINOJSON_NAMESPACE
#endif
namespace ARDUINOJSON_NAMESPACE {
class UnsafeCharPointerReader {
  const char* _ptr;
 public:
  explicit UnsafeCharPointerReader(const char* ptr)
      : _ptr(ptr ? ptr : reinterpret_cast<const char*>("")) {}
  char read() {
    return static_cast<char>(*_ptr++);
  }
  bool ended() const {
    return false;
  }
};
class SafeCharPointerReader {
  const char* _ptr;
  const char* _end;
 public:
  explicit SafeCharPointerReader(const char* ptr, size_t len)
      : _ptr(ptr ? ptr : reinterpret_cast<const char*>("")), _end(_ptr + len) {}
  char read() {
    return static_cast<char>(*_ptr++);
  }
  bool ended() const {
    return _ptr == _end;
  }
};
template <typename TChar>
inline UnsafeCharPointerReader makeReader(TChar* input) {
  return UnsafeCharPointerReader(reinterpret_cast<const char*>(input));
}
template <typename TChar>
inline SafeCharPointerReader makeReader(TChar* input, size_t n) {
  return SafeCharPointerReader(reinterpret_cast<const char*>(input), n);
}
#if ARDUINOJSON_ENABLE_ARDUINO_STRING
inline SafeCharPointerReader makeReader(const ::String& input) {
  return SafeCharPointerReader(input.c_str(), input.length());
}
#endif
}  // namespace ARDUINOJSON_NAMESPACE
#if ARDUINOJSON_ENABLE_STD_STREAM
#include <ostream>
#endif
namespace ARDUINOJSON_NAMESPACE {
class DeserializationError {
  typedef void (DeserializationError::*bool_type)() const;
  void safeBoolHelper() const {}
 public:
  enum Code {
    Ok,
    IncompleteInput,
    InvalidInput,
    NoMemory,
    NotSupported,
    TooDeep
  };
  DeserializationError() {}
  DeserializationError(Code c) : _code(c) {}
  friend bool operator==(const DeserializationError& lhs,
                         const DeserializationError& rhs) {
    return lhs._code == rhs._code;
  }
  friend bool operator!=(const DeserializationError& lhs,
                         const DeserializationError& rhs) {
    return lhs._code != rhs._code;
  }
  friend bool operator==(const DeserializationError& lhs, Code rhs) {
    return lhs._code == rhs;
  }
  friend bool operator==(Code lhs, const DeserializationError& rhs) {
    return lhs == rhs._code;
  }
  friend bool operator!=(const DeserializationError& lhs, Code rhs) {
    return lhs._code != rhs;
  }
  friend bool operator!=(Code lhs, const DeserializationError& rhs) {
    return lhs != rhs._code;
  }
  operator bool_type() const {
    return _code != Ok ? &DeserializationError::safeBoolHelper : 0;
  }
  friend bool operator==(bool value, const DeserializationError& err) {
    return static_cast<bool>(err) == value;
  }
  friend bool operator==(const DeserializationError& err, bool value) {
    return static_cast<bool>(err) == value;
  }
  friend bool operator!=(bool value, const DeserializationError& err) {
    return static_cast<bool>(err) != value;
  }
  friend bool operator!=(const DeserializationError& err, bool value) {
    return static_cast<bool>(err) != value;
  }
  Code code() const {
    return _code;
  }
  const char* c_str() const {
    switch (_code) {
      case Ok:
        return "Ok";
      case TooDeep:
        return "TooDeep";
      case NoMemory:
        return "NoMemory";
      case InvalidInput:
        return "InvalidInput";
      case IncompleteInput:
        return "IncompleteInput";
      case NotSupported:
        return "NotSupported";
      default:
        return "???";
    }
  }
 private:
  Code _code;
};
#if ARDUINOJSON_ENABLE_STD_STREAM
inline std::ostream& operator<<(std::ostream& s,
                                const DeserializationError& e) {
  s << e.c_str();
  return s;
}
inline std::ostream& operator<<(std::ostream& s, DeserializationError::Code c) {
  s << DeserializationError(c).c_str();
  return s;
}
#endif
}  // namespace ARDUINOJSON_NAMESPACE
#if ARDUINOJSON_ENABLE_PROGMEM
namespace ARDUINOJSON_NAMESPACE {
class UnsafeFlashStringReader {
  const char* _ptr;
 public:
  explicit UnsafeFlashStringReader(const __FlashStringHelper* ptr)
      : _ptr(reinterpret_cast<const char*>(ptr)) {}
  char read() {
    return pgm_read_byte_near(_ptr++);
  }
  bool ended() const {
    return false;
  }
};
class SafeFlashStringReader {
  const char* _ptr;
  const char* _end;
 public:
  explicit SafeFlashStringReader(const __FlashStringHelper* ptr, size_t size)
      : _ptr(reinterpret_cast<const char*>(ptr)), _end(_ptr + size) {}
  char read() {
    return pgm_read_byte_near(_ptr++);
  }
  bool ended() const {
    return _ptr == _end;
  }
};
inline UnsafeFlashStringReader makeReader(const __FlashStringHelper* input) {
  return UnsafeFlashStringReader(input);
}
inline SafeFlashStringReader makeReader(const __FlashStringHelper* input,
                                        size_t size) {
  return SafeFlashStringReader(input, size);
}
}  // namespace ARDUINOJSON_NAMESPACE
#endif
namespace ARDUINOJSON_NAMESPACE {
template <typename TIterator>
class IteratorReader {
  TIterator _ptr, _end;
 public:
  explicit IteratorReader(TIterator begin, TIterator end)
      : _ptr(begin), _end(end) {}
  bool ended() const {
    return _ptr == _end;
  }
  char read() {
    return char(*_ptr++);
  }
};
template <typename TInput>
inline IteratorReader<typename TInput::const_iterator> makeReader(
    const TInput& input) {
  return IteratorReader<typename TInput::const_iterator>(input.begin(),
                                                         input.end());
}
}  // namespace ARDUINOJSON_NAMESPACE
#if ARDUINOJSON_ENABLE_STD_STREAM
#include <istream>
namespace ARDUINOJSON_NAMESPACE {
class StdStreamReader {
  std::istream& _stream;
  char _current;
 public:
  explicit StdStreamReader(std::istream& stream)
      : _stream(stream), _current(0) {}
  bool ended() const {
    return _stream.eof();
  }
  char read() {
    return static_cast<char>(_stream.get());
  }
 private:
  StdStreamReader& operator=(const StdStreamReader&);  // Visual Studio C4512
};
inline StdStreamReader makeReader(std::istream& input) {
  return StdStreamReader(input);
}
}  // namespace ARDUINOJSON_NAMESPACE
#endif
namespace ARDUINOJSON_NAMESPACE {
template <template <typename, typename> class TDeserializer, typename TReader,
          typename TWriter>
TDeserializer<TReader, TWriter> makeDeserializer(MemoryPool &pool,
                                                 TReader reader, TWriter writer,
                                                 uint8_t nestingLimit) {
  return TDeserializer<TReader, TWriter>(pool, reader, writer, nestingLimit);
}
template <template <typename, typename> class TDeserializer, typename TString>
typename enable_if<!is_array<TString>::value, DeserializationError>::type
deserialize(JsonDocument &doc, const TString &input) {
  doc.clear();
  return makeDeserializer<TDeserializer>(
             doc.memoryPool(), makeReader(input),
             makeStringStorage(doc.memoryPool(), input), doc.nestingLimit)
      .parse(doc.data());
}
template <template <typename, typename> class TDeserializer, typename TChar>
DeserializationError deserialize(JsonDocument &doc, TChar *input) {
  doc.clear();
  return makeDeserializer<TDeserializer>(
             doc.memoryPool(), makeReader(input),
             makeStringStorage(doc.memoryPool(), input), doc.nestingLimit)
      .parse(doc.data());
}
template <template <typename, typename> class TDeserializer, typename TChar>
DeserializationError deserialize(JsonDocument &doc, TChar *input,
                                 size_t inputSize) {
  doc.clear();
  return makeDeserializer<TDeserializer>(
             doc.memoryPool(), makeReader(input, inputSize),
             makeStringStorage(doc.memoryPool(), input), doc.nestingLimit)
      .parse(doc.data());
}
template <template <typename, typename> class TDeserializer, typename TStream>
DeserializationError deserialize(JsonDocument &doc, TStream &input) {
  doc.clear();
  return makeDeserializer<TDeserializer>(
             doc.memoryPool(), makeReader(input),
             makeStringStorage(doc.memoryPool(), input), doc.nestingLimit)
      .parse(doc.data());
}
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
inline bool isFloat(const char* s) {
  if (!s) return false;
  if (!strcmp(s, "NaN")) return true;
  if (issign(*s)) s++;
  if (!strcmp(s, "Infinity")) return true;
  if (*s == '\0') return false;
  while (isdigit(*s)) s++;
  if (*s == '.') {
    s++;
    while (isdigit(*s)) s++;
  }
  if (*s == 'e' || *s == 'E') {
    s++;
    if (issign(*s)) s++;
    if (!isdigit(*s)) return false;
    while (isdigit(*s)) s++;
  }
  return *s == '\0';
}
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
inline bool isInteger(const char* s) {
  if (!s || !*s) return false;
  if (issign(*s)) s++;
  while (isdigit(*s)) s++;
  return *s == '\0';
}
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
class EscapeSequence {
 public:
  static char escapeChar(char c) {
    const char *p = escapeTable(false);
    while (p[0] && p[1] != c) {
      p += 2;
    }
    return p[0];
  }
  static char unescapeChar(char c) {
    const char *p = escapeTable(true);
    for (;;) {
      if (p[0] == '\0') return c;
      if (p[0] == c) return p[1];
      p += 2;
    }
  }
 private:
  static const char *escapeTable(bool excludeIdenticals) {
    return &"\"\"\\\\b\bf\fn\nr\rt\t"[excludeIdenticals ? 4 : 0];
  }
};
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
template <typename TReader, typename TStringStorage>
class JsonDeserializer {
  typedef typename remove_reference<TStringStorage>::type::StringBuilder
      StringBuilder;
  typedef const char *StringType;
 public:
  JsonDeserializer(MemoryPool &pool, TReader reader,
                   TStringStorage stringStorage, uint8_t nestingLimit)
      : _pool(&pool),
        _reader(reader),
        _stringStorage(stringStorage),
        _nestingLimit(nestingLimit),
        _loaded(false) {}
  DeserializationError parse(VariantData &variant) {
    DeserializationError err = skipSpacesAndComments();
    if (err) return err;
    switch (current()) {
      case '[':
        return parseArray(variant.toArray());
      case '{':
        return parseObject(variant.toObject());
      default:
        return parseValue(variant);
    }
  }
 private:
  JsonDeserializer &operator=(const JsonDeserializer &);  // non-copiable
  char current() {
    if (!_loaded) {
      if (_reader.ended())
        _current = 0;
      else
        _current = _reader.read();
      _loaded = true;
    }
    return _current;
  }
  void move() {
    _loaded = false;
  }
  FORCE_INLINE bool eat(char charToSkip) {
    if (current() != charToSkip) return false;
    move();
    return true;
  }
  DeserializationError parseArray(CollectionData &array) {
    if (_nestingLimit == 0) return DeserializationError::TooDeep;
    if (!eat('[')) return DeserializationError::InvalidInput;
    DeserializationError err = skipSpacesAndComments();
    if (err) return err;
    if (eat(']')) return DeserializationError::Ok;
    for (;;) {
      VariantData *value = array.add(_pool);
      if (!value) return DeserializationError::NoMemory;
      _nestingLimit--;
      err = parse(*value);
      _nestingLimit++;
      if (err) return err;
      err = skipSpacesAndComments();
      if (err) return err;
      if (eat(']')) return DeserializationError::Ok;
      if (!eat(',')) return DeserializationError::InvalidInput;
    }
  }
  DeserializationError parseObject(CollectionData &object) {
    if (_nestingLimit == 0) return DeserializationError::TooDeep;
    if (!eat('{')) return DeserializationError::InvalidInput;
    DeserializationError err = skipSpacesAndComments();
    if (err) return err;
    if (eat('}')) return DeserializationError::Ok;
    for (;;) {
      VariantSlot *slot = object.addSlot(_pool);
      if (!slot) return DeserializationError::NoMemory;
      StringType key;
      err = parseKey(key);
      if (err) return err;
      slot->setOwnedKey(key);
      err = skipSpacesAndComments();
      if (err) return err;  // Colon
      if (!eat(':')) return DeserializationError::InvalidInput;
      _nestingLimit--;
      err = parse(*slot->data());
      _nestingLimit++;
      if (err) return err;
      err = skipSpacesAndComments();
      if (err) return err;
      if (eat('}')) return DeserializationError::Ok;
      if (!eat(',')) return DeserializationError::InvalidInput;
      err = skipSpacesAndComments();
      if (err) return err;
    }
  }
  DeserializationError parseValue(VariantData &variant) {
    if (isQuote(current())) {
      return parseStringValue(variant);
    } else {
      return parseNumericValue(variant);
    }
  }
  DeserializationError parseKey(StringType &key) {
    if (isQuote(current())) {
      return parseQuotedString(key);
    } else {
      return parseNonQuotedString(key);
    }
  }
  DeserializationError parseStringValue(VariantData &variant) {
    StringType value;
    DeserializationError err = parseQuotedString(value);
    if (err) return err;
    variant.setOwnedString(value);
    return DeserializationError::Ok;
  }
  DeserializationError parseQuotedString(StringType &result) {
    StringBuilder builder = _stringStorage.startString();
    const char stopChar = current();
    move();
    for (;;) {
      char c = current();
      move();
      if (c == stopChar) break;
      if (c == '\0') return DeserializationError::IncompleteInput;
      if (c == '\\') {
        c = current();
        if (c == '\0') return DeserializationError::IncompleteInput;
        if (c == 'u') return DeserializationError::NotSupported;
        c = EscapeSequence::unescapeChar(c);
        if (c == '\0') return DeserializationError::InvalidInput;
        move();
      }
      builder.append(c);
    }
    result = builder.complete();
    if (!result) return DeserializationError::NoMemory;
    return DeserializationError::Ok;
  }
  DeserializationError parseNonQuotedString(StringType &result) {
    StringBuilder builder = _stringStorage.startString();
    char c = current();
    if (c == '\0') return DeserializationError::IncompleteInput;
    if (canBeInNonQuotedString(c)) {  // no quotes
      do {
        move();
        builder.append(c);
        c = current();
      } while (canBeInNonQuotedString(c));
    } else {
      return DeserializationError::InvalidInput;
    }
    result = builder.complete();
    if (!result) return DeserializationError::NoMemory;
    return DeserializationError::Ok;
  }
  DeserializationError parseNumericValue(VariantData &result) {
    char buffer[64];
    uint8_t n = 0;
    char c = current();
    while (canBeInNonQuotedString(c) && n < 63) {
      move();
      buffer[n++] = c;
      c = current();
    }
    buffer[n] = 0;
    if (isInteger(buffer)) {
      result.setInteger(parseInteger<Integer>(buffer));
    } else if (isFloat(buffer)) {
      result.setFloat(parseFloat<Float>(buffer));
    } else if (!strcmp(buffer, "true")) {
      result.setBoolean(true);
    } else if (!strcmp(buffer, "false")) {
      result.setBoolean(false);
    } else if (!strcmp(buffer, "null")) {
    } else {
      return DeserializationError::InvalidInput;
    }
    return DeserializationError::Ok;
  }
  static inline bool isBetween(char c, char min, char max) {
    return min <= c && c <= max;
  }
  static inline bool canBeInNonQuotedString(char c) {
    return isBetween(c, '0', '9') || isBetween(c, '_', 'z') ||
           isBetween(c, 'A', 'Z') || c == '+' || c == '-' || c == '.';
  }
  static inline bool isQuote(char c) {
    return c == '\'' || c == '\"';
  }
  DeserializationError skipSpacesAndComments() {
    for (;;) {
      switch (current()) {
        case '\0':
          return DeserializationError::IncompleteInput;
        case ' ':
        case '\t':
        case '\r':
        case '\n':
          move();
          continue;
        case '/':
          move();  // skip '/'
          switch (current()) {
            case '*': {
              move();  // skip '*'
              bool wasStar = false;
              for (;;) {
                char c = current();
                if (c == '\0') return DeserializationError::IncompleteInput;
                if (c == '/' && wasStar) {
                  move();
                  break;
                }
                wasStar = c == '*';
                move();
              }
              break;
            }
            case '/':
              for (;;) {
                move();
                char c = current();
                if (c == '\0') return DeserializationError::IncompleteInput;
                if (c == '\n') break;
              }
              break;
            default:
              return DeserializationError::InvalidInput;
          }
          break;
        default:
          return DeserializationError::Ok;
      }
    }
  }
  MemoryPool *_pool;
  TReader _reader;
  TStringStorage _stringStorage;
  uint8_t _nestingLimit;
  char _current;
  bool _loaded;
};
template <typename TDocument, typename TInput>
DeserializationError deserializeJson(TDocument &doc, const TInput &input) {
  return deserialize<JsonDeserializer>(doc, input);
}
template <typename TDocument, typename TInput>
DeserializationError deserializeJson(TDocument &doc, TInput *input) {
  return deserialize<JsonDeserializer>(doc, input);
}
template <typename TDocument, typename TInput>
DeserializationError deserializeJson(TDocument &doc, TInput *input,
                                     size_t inputSize) {
  return deserialize<JsonDeserializer>(doc, input, inputSize);
}
template <typename TDocument, typename TInput>
DeserializationError deserializeJson(TDocument &doc, TInput &input) {
  return deserialize<JsonDeserializer>(doc, input);
}
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
class DummyWriter {
 public:
  size_t write(uint8_t) {
    return 1;
  }
  size_t write(const uint8_t*, size_t n) {
    return n;
  }
};
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
template <template <typename> class TSerializer, typename TSource>
size_t measure(const TSource &source) {
  DummyWriter dp;
  TSerializer<DummyWriter> serializer(dp);
  source.accept(serializer);
  return serializer.bytesWritten();
}
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
class StaticStringWriter {
 public:
  StaticStringWriter(char *buf, size_t size) : end(buf + size - 1), p(buf) {
    *p = '\0';
  }
  size_t write(uint8_t c) {
    if (p >= end) return 0;
    *p++ = static_cast<char>(c);
    *p = '\0';
    return 1;
  }
  size_t write(const uint8_t *s, size_t n) {
    char *begin = p;
    while (p < end && n > 0) {
      *p++ = static_cast<char>(*s++);
      n--;
    }
    *p = '\0';
    return size_t(p - begin);
  }
 private:
  char *end;
  char *p;
};
}  // namespace ARDUINOJSON_NAMESPACE
#if ARDUINOJSON_ENABLE_STD_STREAM
#if ARDUINOJSON_ENABLE_STD_STREAM
namespace ARDUINOJSON_NAMESPACE {
class StreamWriter {
 public:
  explicit StreamWriter(std::ostream& os) : _os(os) {}
  size_t write(uint8_t c) {
    _os << c;
    return 1;
  }
  size_t write(const uint8_t* s, size_t n) {
    _os.write(reinterpret_cast<const char*>(s),
              static_cast<std::streamsize>(n));
    return n;
  }
 private:
  StreamWriter& operator=(const StreamWriter&);
  std::ostream& _os;
};
}  // namespace ARDUINOJSON_NAMESPACE
#endif  // ARDUINOJSON_ENABLE_STD_STREAM
#endif
namespace ARDUINOJSON_NAMESPACE {
template <template <typename> class TSerializer, typename TSource,
          typename TPrint>
typename enable_if<!IsWriteableString<TPrint>::value, size_t>::type serialize(
    const TSource &source, TPrint &destination) {
  TSerializer<TPrint> serializer(destination);
  source.accept(serializer);
  return serializer.bytesWritten();
}
#if ARDUINOJSON_ENABLE_STD_STREAM
template <template <typename> class TSerializer, typename TSource>
size_t serialize(const TSource &source, std::ostream &os) {
  StreamWriter writer(os);
  return serialize<TSerializer>(source, writer);
}
#endif
template <template <typename> class TSerializer, typename TSource>
size_t serialize(const TSource &source, char *buffer, size_t bufferSize) {
  StaticStringWriter writer(buffer, bufferSize);
  return serialize<TSerializer>(source, writer);
}
template <template <typename> class TSerializer, typename TSource, size_t N>
size_t serialize(const TSource &source, char (&buffer)[N]) {
  StaticStringWriter writer(buffer, N);
  return serialize<TSerializer>(source, writer);
}
template <template <typename> class TSerializer, typename TSource,
          typename TString>
typename enable_if<IsWriteableString<TString>::value, size_t>::type serialize(
    const TSource &source, TString &str) {
  DynamicStringWriter<TString> writer(str);
  return serialize<TSerializer>(source, writer);
}
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
template <typename TFloat>
struct FloatParts {
  uint32_t integral;
  uint32_t decimal;
  int16_t exponent;
  int8_t decimalPlaces;
  FloatParts(TFloat value) {
    uint32_t maxDecimalPart = sizeof(TFloat) >= 8 ? 1000000000 : 1000000;
    decimalPlaces = sizeof(TFloat) >= 8 ? 9 : 6;
    exponent = normalize(value);
    integral = uint32_t(value);
    for (uint32_t tmp = integral; tmp >= 10; tmp /= 10) {
      maxDecimalPart /= 10;
      decimalPlaces--;
    }
    TFloat remainder = (value - TFloat(integral)) * TFloat(maxDecimalPart);
    decimal = uint32_t(remainder);
    remainder = remainder - TFloat(decimal);
    decimal += uint32_t(remainder * 2);
    if (decimal >= maxDecimalPart) {
      decimal = 0;
      integral++;
      if (exponent && integral >= 10) {
        exponent++;
        integral = 1;
      }
    }
    while (decimal % 10 == 0 && decimalPlaces > 0) {
      decimal /= 10;
      decimalPlaces--;
    }
  }
  static int16_t normalize(TFloat& value) {
    typedef FloatTraits<TFloat> traits;
    int16_t powersOf10 = 0;
    int8_t index = sizeof(TFloat) == 8 ? 8 : 5;
    int bit = 1 << index;
    if (value >= ARDUINOJSON_POSITIVE_EXPONENTIATION_THRESHOLD) {
      for (; index >= 0; index--) {
        if (value >= traits::positiveBinaryPowerOfTen(index)) {
          value *= traits::negativeBinaryPowerOfTen(index);
          powersOf10 = int16_t(powersOf10 + bit);
        }
        bit >>= 1;
      }
    }
    if (value > 0 && value <= ARDUINOJSON_NEGATIVE_EXPONENTIATION_THRESHOLD) {
      for (; index >= 0; index--) {
        if (value < traits::negativeBinaryPowerOfTenPlusOne(index)) {
          value *= traits::positiveBinaryPowerOfTen(index);
          powersOf10 = int16_t(powersOf10 - bit);
        }
        bit >>= 1;
      }
    }
    return powersOf10;
  }
};
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
template <typename TWriter>
class JsonWriter {
 public:
  explicit JsonWriter(TWriter &writer) : _writer(writer), _length(0) {}
  size_t bytesWritten() const {
    return _length;
  }
  void beginArray() {
    writeRaw('[');
  }
  void endArray() {
    writeRaw(']');
  }
  void beginObject() {
    writeRaw('{');
  }
  void endObject() {
    writeRaw('}');
  }
  void writeColon() {
    writeRaw(':');
  }
  void writeComma() {
    writeRaw(',');
  }
  void writeBoolean(bool value) {
    if (value)
      writeRaw("true");
    else
      writeRaw("false");
  }
  void writeString(const char *value) {
    if (!value) {
      writeRaw("null");
    } else {
      writeRaw('\"');
      while (*value) writeChar(*value++);
      writeRaw('\"');
    }
  }
  void writeChar(char c) {
    char specialChar = EscapeSequence::escapeChar(c);
    if (specialChar) {
      writeRaw('\\');
      writeRaw(specialChar);
    } else {
      writeRaw(c);
    }
  }
  template <typename T>
  void writeFloat(T value) {
    if (isnan(value)) return writeRaw("NaN");
    if (value < 0.0) {
      writeRaw('-');
      value = -value;
    }
    if (isinf(value)) return writeRaw("Infinity");
    FloatParts<T> parts(value);
    writeInteger(parts.integral);
    if (parts.decimalPlaces) writeDecimals(parts.decimal, parts.decimalPlaces);
    if (parts.exponent < 0) {
      writeRaw("e-");
      writeInteger(-parts.exponent);
    }
    if (parts.exponent > 0) {
      writeRaw('e');
      writeInteger(parts.exponent);
    }
  }
  template <typename T>
  void writeInteger(T value) {
    char buffer[22];
    char *end = buffer + sizeof(buffer);
    char *begin = end;
    do {
      *--begin = char(value % 10 + '0');
      value = T(value / 10);
    } while (value);
    writeRaw(begin, end);
  }
  void writeDecimals(uint32_t value, int8_t width) {
    char buffer[16];
    char *end = buffer + sizeof(buffer);
    char *begin = end;
    while (width--) {
      *--begin = char(value % 10 + '0');
      value /= 10;
    }
    *--begin = '.';
    writeRaw(begin, end);
  }
  void writeRaw(const char *s) {
    _length += _writer.write(reinterpret_cast<const uint8_t *>(s), strlen(s));
  }
  void writeRaw(const char *begin, const char *end) {
    _length += _writer.write(reinterpret_cast<const uint8_t *>(begin),
                             static_cast<size_t>(end - begin));
  }
  template <size_t N>
  void writeRaw(const char (&s)[N]) {
    _length += _writer.write(reinterpret_cast<const uint8_t *>(s), N - 1);
  }
  void writeRaw(char c) {
    _length += _writer.write(static_cast<uint8_t>(c));
  }
 protected:
  TWriter &_writer;
  size_t _length;
 private:
  JsonWriter &operator=(const JsonWriter &);  // cannot be assigned
};
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
template <typename TWriter>
class JsonSerializer {
 public:
  JsonSerializer(TWriter &writer) : _writer(writer) {}
  void visitFloat(Float value) {
    _writer.writeFloat(value);
  }
  void visitArray(const CollectionData &array) {
    _writer.beginArray();
    VariantSlot *slot = array.head();
    while (slot != 0) {
      slot->data()->accept(*this);
      slot = slot->next();
      if (slot == 0) break;
      _writer.writeComma();
    }
    _writer.endArray();
  }
  void visitObject(const CollectionData &object) {
    _writer.beginObject();
    VariantSlot *slot = object.head();
    while (slot != 0) {
      _writer.writeString(slot->key());
      _writer.writeColon();
      slot->data()->accept(*this);
      slot = slot->next();
      if (slot == 0) break;
      _writer.writeComma();
    }
    _writer.endObject();
  }
  void visitString(const char *value) {
    _writer.writeString(value);
  }
  void visitRawJson(const char *data, size_t n) {
    for (size_t i = 0; i < n; i++) _writer.writeRaw(data[i]);
  }
  void visitNegativeInteger(UInt value) {
    _writer.writeRaw('-');
    _writer.writeInteger(value);
  }
  void visitPositiveInteger(UInt value) {
    _writer.writeInteger(value);
  }
  void visitBoolean(bool value) {
    _writer.writeBoolean(value);
  }
  void visitNull() {
    _writer.writeRaw("null");
  }
  size_t bytesWritten() const {
    return _writer.bytesWritten();
  }
 private:
  JsonWriter<TWriter> _writer;
};
template <typename TSource, typename TDestination>
size_t serializeJson(const TSource &source, TDestination &destination) {
  return serialize<JsonSerializer>(source, destination);
}
template <typename TSource>
size_t serializeJson(const TSource &source, char *buffer, size_t bufferSize) {
  return serialize<JsonSerializer>(source, buffer, bufferSize);
}
template <typename TSource>
size_t measureJson(const TSource &source) {
  return measure<JsonSerializer>(source);
}
#if ARDUINOJSON_ENABLE_STD_STREAM
template <typename T>
inline typename enable_if<IsVisitable<T>::value, std::ostream &>::type
operator<<(std::ostream &os, const T &source) {
  serializeJson(source, os);
  return os;
}
#endif
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
template <typename Print>
class IndentedPrint {
 public:
  explicit IndentedPrint(Print &p) : sink(&p) {
    level = 0;
    tabSize = 2;
    isNewLine = true;
  }
  size_t write(uint8_t c) {
    size_t n = 0;
    if (isNewLine) n += writeTabs();
    n += sink->write(c);
    isNewLine = c == '\n';
    return n;
  }
  size_t write(const uint8_t *s, size_t n) {
    size_t bytesWritten = 0;
    while (n > 0) {
      bytesWritten += write(*s++);
      n--;
    }
    return bytesWritten;
  }
  void indent() {
    if (level < MAX_LEVEL) level++;
  }
  void unindent() {
    if (level > 0) level--;
  }
  void setTabSize(uint8_t n) {
    if (n < MAX_TAB_SIZE) tabSize = n & MAX_TAB_SIZE;
  }
 private:
  Print *sink;
  uint8_t level : 4;
  uint8_t tabSize : 3;
  bool isNewLine : 1;
  size_t writeTabs() {
    size_t n = 0;
    for (int i = 0; i < level * tabSize; i++) n += sink->write(' ');
    return n;
  }
  static const int MAX_LEVEL = 15;    // because it's only 4 bits
  static const int MAX_TAB_SIZE = 7;  // because it's only 3 bits
};
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
template <typename TWriter>
class Prettyfier {
 public:
  explicit Prettyfier(IndentedPrint<TWriter>& p) : _sink(p) {
    _previousChar = 0;
    _inString = false;
  }
  size_t write(uint8_t c) {
    size_t n = _inString ? handleStringChar(c) : handleMarkupChar(char(c));
    _previousChar = char(c);
    return n;
  }
  size_t write(const uint8_t* s, size_t n) {
    size_t bytesWritten = 0;
    while (n > 0) {
      bytesWritten += write(*s++);
      n--;
    }
    return bytesWritten;
  }
 private:
  Prettyfier& operator=(const Prettyfier&);  // cannot be assigned
  bool inEmptyBlock() {
    return _previousChar == '{' || _previousChar == '[';
  }
  size_t handleStringChar(uint8_t c) {
    bool isQuote = c == '"' && _previousChar != '\\';
    if (isQuote) _inString = false;
    return _sink.write(c);
  }
  size_t handleMarkupChar(char c) {
    switch (c) {
      case '{':
      case '[':
        return writeBlockOpen(c);
      case '}':
      case ']':
        return writeBlockClose(c);
      case ':':
        return writeColon();
      case ',':
        return writeComma();
      case '"':
        return writeQuoteOpen();
      default:
        return writeNormalChar(c);
    }
  }
  size_t writeBlockClose(char c) {
    size_t n = 0;
    n += unindentIfNeeded();
    n += write(c);
    return n;
  }
  size_t writeBlockOpen(char c) {
    size_t n = 0;
    n += indentIfNeeded();
    n += write(c);
    return n;
  }
  size_t writeColon() {
    size_t n = 0;
    n += write(": ");
    return n;
  }
  size_t writeComma() {
    size_t n = 0;
    n += write(",\r\n");
    return n;
  }
  size_t writeQuoteOpen() {
    _inString = true;
    size_t n = 0;
    n += indentIfNeeded();
    n += write('"');
    return n;
  }
  size_t writeNormalChar(char c) {
    size_t n = 0;
    n += indentIfNeeded();
    n += write(c);
    return n;
  }
  size_t indentIfNeeded() {
    if (!inEmptyBlock()) return 0;
    _sink.indent();
    return write("\r\n");
  }
  size_t unindentIfNeeded() {
    if (inEmptyBlock()) return 0;
    _sink.unindent();
    return write("\r\n");
  }
  size_t write(char c) {
    return _sink.write(static_cast<uint8_t>(c));
  }
  template <size_t N>
  size_t write(const char (&s)[N]) {
    return _sink.write(reinterpret_cast<const uint8_t*>(s), N - 1);
  }
  char _previousChar;
  IndentedPrint<TWriter>& _sink;
  bool _inString;
};
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
template <typename TPrint>
class PrettyJsonSerializer_Base {
 public:
  PrettyJsonSerializer_Base(TPrint &output)
      : _indentedPrint(output), _prettyfier(_indentedPrint) {}
 protected:
  IndentedPrint<TPrint> _indentedPrint;
  Prettyfier<TPrint> _prettyfier;
};
template <typename TPrint>
class PrettyJsonSerializer : PrettyJsonSerializer_Base<TPrint>,
                             public JsonSerializer<Prettyfier<TPrint> > {
 public:
  PrettyJsonSerializer(TPrint &output)
      : PrettyJsonSerializer_Base<TPrint>(output),
        JsonSerializer<Prettyfier<TPrint> >(
            PrettyJsonSerializer_Base<TPrint>::_prettyfier) {}
};
template <typename TSource, typename TDestination>
size_t serializeJsonPretty(TSource &source, TDestination &destination) {
  return serialize<PrettyJsonSerializer>(source, destination);
}
template <typename TSource>
size_t serializeJsonPretty(const TSource &source, char *buffer,
                           size_t bufferSize) {
  return serialize<PrettyJsonSerializer>(source, buffer, bufferSize);
}
template <typename TSource>
size_t measureJsonPretty(const TSource &source) {
  return measure<PrettyJsonSerializer>(source);
}
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
template <typename T>
inline void swap(T& a, T& b) {
  T t(a);
  a = b;
  b = t;
}
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
inline void fixEndianess(uint8_t* p, integral_constant<size_t, 8>) {
  swap(p[0], p[7]);
  swap(p[1], p[6]);
  swap(p[2], p[5]);
  swap(p[3], p[4]);
}
inline void fixEndianess(uint8_t* p, integral_constant<size_t, 4>) {
  swap(p[0], p[3]);
  swap(p[1], p[2]);
}
inline void fixEndianess(uint8_t* p, integral_constant<size_t, 2>) {
  swap(p[0], p[1]);
}
inline void fixEndianess(uint8_t*, integral_constant<size_t, 1>) {}
template <typename T>
inline void fixEndianess(T& value) {
#if ARDUINOJSON_LITTLE_ENDIAN
  fixEndianess(reinterpret_cast<uint8_t*>(&value),
               integral_constant<size_t, sizeof(T)>());
#endif
}
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
inline void doubleToFloat(const uint8_t d[8], uint8_t f[4]) {
  f[0] = uint8_t((d[0] & 0xC0) | (d[0] << 3 & 0x3f) | (d[1] >> 5));
  f[1] = uint8_t((d[1] << 3) | (d[2] >> 5));
  f[2] = uint8_t((d[2] << 3) | (d[3] >> 5));
  f[3] = uint8_t((d[3] << 3) | (d[4] >> 5));
}
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
template <typename TReader, typename TStringStorage>
class MsgPackDeserializer {
  typedef typename remove_reference<TStringStorage>::type::StringBuilder
      StringBuilder;
  typedef const char *StringType;
 public:
  MsgPackDeserializer(MemoryPool &pool, TReader reader,
                      TStringStorage stringStorage, uint8_t nestingLimit)
      : _pool(&pool),
        _reader(reader),
        _stringStorage(stringStorage),
        _nestingLimit(nestingLimit) {}
  DeserializationError parse(VariantData &variant) {
    uint8_t code;
    if (!readByte(code)) return DeserializationError::IncompleteInput;
    if ((code & 0x80) == 0) {
      variant.setUnsignedInteger(code);
      return DeserializationError::Ok;
    }
    if ((code & 0xe0) == 0xe0) {
      variant.setSignedInteger(static_cast<int8_t>(code));
      return DeserializationError::Ok;
    }
    if ((code & 0xe0) == 0xa0) {
      return readString(variant, code & 0x1f);
    }
    if ((code & 0xf0) == 0x90) {
      return readArray(variant.toArray(), code & 0x0F);
    }
    if ((code & 0xf0) == 0x80) {
      return readObject(variant.toObject(), code & 0x0F);
    }
    switch (code) {
      case 0xc0:
        return DeserializationError::Ok;
      case 0xc2:
        variant.setBoolean(false);
        return DeserializationError::Ok;
      case 0xc3:
        variant.setBoolean(true);
        return DeserializationError::Ok;
      case 0xcc:
        return readInteger<uint8_t>(variant);
      case 0xcd:
        return readInteger<uint16_t>(variant);
      case 0xce:
        return readInteger<uint32_t>(variant);
      case 0xcf:
#if ARDUINOJSON_USE_LONG_LONG
        return readInteger<uint64_t>(variant);
#else
        readInteger<uint32_t>();
        return readInteger<uint32_t>(variant);
#endif
      case 0xd0:
        return readInteger<int8_t>(variant);
      case 0xd1:
        return readInteger<int16_t>(variant);
      case 0xd2:
        return readInteger<int32_t>(variant);
      case 0xd3:
#if ARDUINOJSON_USE_LONG_LONG
        return readInteger<int64_t>(variant);
#else
        if (!skip(4)) return DeserializationError::IncompleteInput;
        return readInteger<int32_t>(variant);
#endif
      case 0xca:
        return readFloat<float>(variant);
      case 0xcb:
        return readDouble<double>(variant);
      case 0xd9:
        return readString<uint8_t>(variant);
      case 0xda:
        return readString<uint16_t>(variant);
      case 0xdb:
        return readString<uint32_t>(variant);
      case 0xdc:
        return readArray<uint16_t>(variant.toArray());
      case 0xdd:
        return readArray<uint32_t>(variant.toArray());
      case 0xde:
        return readObject<uint16_t>(variant.toObject());
      case 0xdf:
        return readObject<uint32_t>(variant.toObject());
      default:
        return DeserializationError::NotSupported;
    }
  }
 private:
  MsgPackDeserializer &operator=(const MsgPackDeserializer &);
  bool skip(uint8_t n) {
    while (n--) {
      if (_reader.ended()) return false;
      _reader.read();
    }
    return true;
  }
  bool readByte(uint8_t &value) {
    if (_reader.ended()) return false;
    value = static_cast<uint8_t>(_reader.read());
    return true;
  }
  bool readBytes(uint8_t *p, size_t n) {
    for (size_t i = 0; i < n; i++) {
      if (!readByte(p[i])) return false;
    }
    return true;
  }
  template <typename T>
  bool readBytes(T &value) {
    return readBytes(reinterpret_cast<uint8_t *>(&value), sizeof(value));
  }
  template <typename T>
  T readInteger() {
    T value;
    readBytes(value);
    fixEndianess(value);
    return value;
  }
  template <typename T>
  bool readInteger(T &value) {
    if (!readBytes(value)) return false;
    fixEndianess(value);
    return true;
  }
  template <typename T>
  DeserializationError readInteger(VariantData &variant) {
    T value;
    if (!readInteger(value)) return DeserializationError::IncompleteInput;
    variant.setInteger(value);
    return DeserializationError::Ok;
  }
  template <typename T>
  typename enable_if<sizeof(T) == 4, DeserializationError>::type readFloat(
      VariantData &variant) {
    T value;
    if (!readBytes(value)) return DeserializationError::IncompleteInput;
    fixEndianess(value);
    variant.setFloat(value);
    return DeserializationError::Ok;
  }
  template <typename T>
  typename enable_if<sizeof(T) == 8, DeserializationError>::type readDouble(
      VariantData &variant) {
    T value;
    if (!readBytes(value)) return DeserializationError::IncompleteInput;
    fixEndianess(value);
    variant.setFloat(value);
    return DeserializationError::Ok;
  }
  template <typename T>
  typename enable_if<sizeof(T) == 4, DeserializationError>::type readDouble(
      VariantData &variant) {
    uint8_t i[8];  // input is 8 bytes
    T value;       // output is 4 bytes
    uint8_t *o = reinterpret_cast<uint8_t *>(&value);
    if (!readBytes(i, 8)) return DeserializationError::IncompleteInput;
    doubleToFloat(i, o);
    fixEndianess(value);
    variant.setFloat(value);
    return DeserializationError::Ok;
  }
  template <typename T>
  DeserializationError readString(VariantData &variant) {
    T size;
    if (!readInteger(size)) return DeserializationError::IncompleteInput;
    return readString(variant, size);
  }
  template <typename T>
  DeserializationError readString(StringType &str) {
    T size;
    if (!readInteger(size)) return DeserializationError::IncompleteInput;
    return readString(str, size);
  }
  DeserializationError readString(VariantData &variant, size_t n) {
    StringType s;
    DeserializationError err = readString(s, n);
    if (!err) variant.setOwnedString(s);
    return err;
  }
  DeserializationError readString(StringType &result, size_t n) {
    StringBuilder builder = _stringStorage.startString();
    for (; n; --n) {
      uint8_t c;
      if (!readBytes(c)) return DeserializationError::IncompleteInput;
      builder.append(static_cast<char>(c));
    }
    result = builder.complete();
    if (!result) return DeserializationError::NoMemory;
    return DeserializationError::Ok;
  }
  template <typename TSize>
  DeserializationError readArray(CollectionData &array) {
    TSize size;
    if (!readInteger(size)) return DeserializationError::IncompleteInput;
    return readArray(array, size);
  }
  DeserializationError readArray(CollectionData &array, size_t n) {
    if (_nestingLimit == 0) return DeserializationError::TooDeep;
    --_nestingLimit;
    for (; n; --n) {
      VariantData *value = array.add(_pool);
      if (!value) return DeserializationError::NoMemory;
      DeserializationError err = parse(*value);
      if (err) return err;
    }
    ++_nestingLimit;
    return DeserializationError::Ok;
  }
  template <typename TSize>
  DeserializationError readObject(CollectionData &object) {
    TSize size;
    if (!readInteger(size)) return DeserializationError::IncompleteInput;
    return readObject(object, size);
  }
  DeserializationError readObject(CollectionData &object, size_t n) {
    if (_nestingLimit == 0) return DeserializationError::TooDeep;
    --_nestingLimit;
    for (; n; --n) {
      VariantSlot *slot = object.addSlot(_pool);
      if (!slot) return DeserializationError::NoMemory;
      StringType key;
      DeserializationError err = parseKey(key);
      if (err) return err;
      slot->setOwnedKey(key);
      err = parse(*slot->data());
      if (err) return err;
    }
    ++_nestingLimit;
    return DeserializationError::Ok;
  }
  DeserializationError parseKey(StringType &key) {
    uint8_t code;
    if (!readByte(code)) return DeserializationError::IncompleteInput;
    if ((code & 0xe0) == 0xa0) return readString(key, code & 0x1f);
    switch (code) {
      case 0xd9:
        return readString<uint8_t>(key);
      case 0xda:
        return readString<uint16_t>(key);
      case 0xdb:
        return readString<uint32_t>(key);
      default:
        return DeserializationError::NotSupported;
    }
  }
  MemoryPool *_pool;
  TReader _reader;
  TStringStorage _stringStorage;
  uint8_t _nestingLimit;
};
template <typename TDocument, typename TInput>
DeserializationError deserializeMsgPack(TDocument &doc, const TInput &input) {
  return deserialize<MsgPackDeserializer>(doc, input);
}
template <typename TDocument, typename TInput>
DeserializationError deserializeMsgPack(TDocument &doc, TInput *input) {
  return deserialize<MsgPackDeserializer>(doc, input);
}
template <typename TDocument, typename TInput>
DeserializationError deserializeMsgPack(TDocument &doc, TInput *input,
                                        size_t inputSize) {
  return deserialize<MsgPackDeserializer>(doc, input, inputSize);
}
template <typename TDocument, typename TInput>
DeserializationError deserializeMsgPack(TDocument &doc, TInput &input) {
  return deserialize<MsgPackDeserializer>(doc, input);
}
}  // namespace ARDUINOJSON_NAMESPACE
namespace ARDUINOJSON_NAMESPACE {
template <typename TWriter>
class MsgPackSerializer {
 public:
  MsgPackSerializer(TWriter& writer) : _writer(&writer), _bytesWritten(0) {}
  template <typename T>
  typename enable_if<sizeof(T) == 4>::type visitFloat(T value32) {
    writeByte(0xCA);
    writeInteger(value32);
  }
  template <typename T>
  typename enable_if<sizeof(T) == 8>::type visitFloat(T value64) {
    float value32 = float(value64);
    if (value32 == value64) {
      writeByte(0xCA);
      writeInteger(value32);
    } else {
      writeByte(0xCB);
      writeInteger(value64);
    }
  }
  void visitArray(const CollectionData& array) {
    size_t n = array.size();
    if (n < 0x10) {
      writeByte(uint8_t(0x90 + array.size()));
    } else if (n < 0x10000) {
      writeByte(0xDC);
      writeInteger(uint16_t(n));
    } else {
      writeByte(0xDD);
      writeInteger(uint32_t(n));
    }
    for (VariantSlot* slot = array.head(); slot; slot = slot->next()) {
      slot->data()->accept(*this);
    }
  }
  void visitObject(const CollectionData& object) {
    size_t n = object.size();
    if (n < 0x10) {
      writeByte(uint8_t(0x80 + n));
    } else if (n < 0x10000) {
      writeByte(0xDE);
      writeInteger(uint16_t(n));
    } else {
      writeByte(0xDF);
      writeInteger(uint32_t(n));
    }
    for (VariantSlot* slot = object.head(); slot; slot = slot->next()) {
      visitString(slot->key());
      slot->data()->accept(*this);
    }
  }
  void visitString(const char* value) {
    if (!value) return writeByte(0xC0);  // nil
    size_t n = strlen(value);
    if (n < 0x20) {
      writeByte(uint8_t(0xA0 + n));
    } else if (n < 0x100) {
      writeByte(0xD9);
      writeInteger(uint8_t(n));
    } else if (n < 0x10000) {
      writeByte(0xDA);
      writeInteger(uint16_t(n));
    } else {
      writeByte(0xDB);
      writeInteger(uint32_t(n));
    }
    writeBytes(reinterpret_cast<const uint8_t*>(value), n);
  }
  void visitRawJson(const char* data, size_t size) {
    writeBytes(reinterpret_cast<const uint8_t*>(data), size);
  }
  void visitNegativeInteger(UInt value) {
    UInt negated = UInt(~value + 1);
    if (value <= 0x20) {
      writeInteger(int8_t(negated));
    } else if (value <= 0x80) {
      writeByte(0xD0);
      writeInteger(int8_t(negated));
    } else if (value <= 0x8000) {
      writeByte(0xD1);
      writeInteger(int16_t(negated));
    } else if (value <= 0x80000000) {
      writeByte(0xD2);
      writeInteger(int32_t(negated));
    }
#if ARDUINOJSON_USE_LONG_LONG
    else {
      writeByte(0xD3);
      writeInteger(int64_t(negated));
    }
#endif
  }
  void visitPositiveInteger(UInt value) {
    if (value <= 0x7F) {
      writeInteger(uint8_t(value));
    } else if (value <= 0xFF) {
      writeByte(0xCC);
      writeInteger(uint8_t(value));
    } else if (value <= 0xFFFF) {
      writeByte(0xCD);
      writeInteger(uint16_t(value));
    } else if (value <= 0xFFFFFFFF) {
      writeByte(0xCE);
      writeInteger(uint32_t(value));
    }
#if ARDUINOJSON_USE_LONG_LONG
    else {
      writeByte(0xCF);
      writeInteger(uint64_t(value));
    }
#endif
  }
  void visitBoolean(bool value) {
    writeByte(value ? 0xC3 : 0xC2);
  }
  void visitNull() {
    writeByte(0xC0);
  }
  size_t bytesWritten() const {
    return _bytesWritten;
  }
 private:
  void writeByte(uint8_t c) {
    _bytesWritten += _writer->write(c);
  }
  void writeBytes(const uint8_t* p, size_t n) {
    _bytesWritten += _writer->write(p, n);
  }
  template <typename T>
  void writeInteger(T value) {
    fixEndianess(value);
    writeBytes(reinterpret_cast<uint8_t*>(&value), sizeof(value));
  }
  TWriter* _writer;
  size_t _bytesWritten;
};
template <typename TSource, typename TDestination>
inline size_t serializeMsgPack(const TSource& source, TDestination& output) {
  return serialize<MsgPackSerializer>(source, output);
}
template <typename TSource, typename TDestination>
inline size_t serializeMsgPack(const TSource& source, TDestination* output,
                               size_t size) {
  return serialize<MsgPackSerializer>(source, output, size);
}
template <typename TSource>
inline size_t measureMsgPack(const TSource& source) {
  return measure<MsgPackSerializer>(source);
}
}  // namespace ARDUINOJSON_NAMESPACE
namespace ArduinoJson {
typedef ARDUINOJSON_NAMESPACE::ArrayRef JsonArray;
typedef ARDUINOJSON_NAMESPACE::ArrayConstRef JsonArrayConst;
typedef ARDUINOJSON_NAMESPACE::Float JsonFloat;
typedef ARDUINOJSON_NAMESPACE::Integer JsonInteger;
typedef ARDUINOJSON_NAMESPACE::ObjectRef JsonObject;
typedef ARDUINOJSON_NAMESPACE::ObjectConstRef JsonObjectConst;
typedef ARDUINOJSON_NAMESPACE::Pair JsonPair;
typedef ARDUINOJSON_NAMESPACE::UInt JsonUInt;
typedef ARDUINOJSON_NAMESPACE::VariantRef JsonVariant;
typedef ARDUINOJSON_NAMESPACE::VariantConstRef JsonVariantConst;
typedef ARDUINOJSON_NAMESPACE::String JsonString;
using ARDUINOJSON_NAMESPACE::DeserializationError;
using ARDUINOJSON_NAMESPACE::DynamicJsonDocument;
using ARDUINOJSON_NAMESPACE::serialized;
using ARDUINOJSON_NAMESPACE::StaticJsonDocument;
}  // namespace ArduinoJson

using namespace ArduinoJson;

#else

#error ArduinoJson requires a C++ compiler, please change file extension to .cc or .cpp

#endif
