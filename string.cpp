#include "string.h"

#include <assert.h>
#include <algorithm>
#include <cstring>
#include <cwctype>
#include <limits>
#include <vector>

// #include "array.h"
#include "exception.h"
#include "range.h"

const size_t TL::String::kNotFound = std::numeric_limits<size_t>::max();
const size_t TL::String::kMaxLength = std::numeric_limits<size_t>::max() - 1;

// String construtors & destructor
TL::String::String() : length_(0), capacity_(1),
                       str_(new wchar_t[1]) {
  str_[0] = L'\0';
}

TL::String::String(size_t n) : length_(0), capacity_(n+1) {
  if (n > kMaxLength)
    throw std::length_error("String::String(size_t): n > kMaxLength");
  str_ = new wchar_t[capacity_];
  str_[0] = L'\0';
}

TL::String::String(size_t n, wchar_t c) : length_(n), capacity_(n + 1) {
  if (n >= kMaxLength)
    throw std::length_error("String::String(size_t, wchar_t): n > kMaxLength");
  str_ = new wchar_t[capacity_];
  for (size_t i = 0; i < length_; ++i)
    str_[i] = c;
  str_[length_] = L'\0';
}


TL::String::String(const wchar_t* p) {
  if (p != nullptr) {
    length_ = wcslen(p);
    str_ = AllocAndCopyFromStr(p, length_);
    capacity_ = length_ + 1;
  } else {
    throw TL::NullPointer("String::String(const wchar_t*): p");
  }
}

TL::String::String(const String& s) : 
    length_(s.length()),
    capacity_(s.length() + 1),
    str_(AllocAndCopyFromStr(s.str(), s.length())) {
}
/*
template<typename Iter>
TL::String(Iter beg, Iter end) :
    length_(end - beg),
    capacity_(end - beg + 1),
    str_(new wchar_t[end - beg + 1]) {
  std::copy(beg, end, str_);
  str_[length_] = L'\0';
}
*/

TL::String::String(const String& s, size_t start, size_t length) {
  if (start >= s.length()) throw std::out_of_range("String: start >= s.length()");
  if (start + length > s.length())
    length = s.length() - start;
  str_ = AllocAndCopyFromStr(s.str() + start, length);
  length_ = length;
  capacity_ = length_ + 1;
}

TL::String::~String() {
  delete[] str_;
}

// overload functions
TL::String& TL::String::operator=(const String& s) {
  String temp(s);
  Swap(temp);
  return *this;
}

TL::String& TL::String::operator+=(const String& s) {
  return InsertM(length_, s);
}

TL::String& TL::String::operator+=(wchar_t c) {
  return InsertM(length_, c);
}

inline
wchar_t& TL::String::operator[](size_t index) {
  if (index >= length_)
    throw std::out_of_range("String::operator[]");
  return *(str_ + index);
}

inline
const wchar_t& TL::String::operator[](size_t index) const {
  if (index >= length_)
    throw std::out_of_range("String::operator[]");
  return str_[index];
}

void TL::String::Swap(String& s) {
  std::swap(length_, s.length_);
  std::swap(capacity_, s.capacity_);
  std::swap(str_, s.str_);
}

// string algorithm, always return new string(s)
inline
const std::vector<TL::String> TL::String::Split(wchar_t delim) const {
  return std::move(Split(String(1, delim)));
}

const std::vector<TL::String> TL::String::Split(const String& delims) const {
  std::vector<String> result;
  if (!IsEmpty()) {
    size_t beg(0), end(length_ - 1);
    while (beg < length_ && delims.IsContains(str_[beg])) { ++beg; }
    while (end != 0 && delims.IsContains(str_[end])) { --end; }
    bool is_in_delim = false;
    for (size_t i = beg; i <= end; ++i) {
      if (delims.IsContains(str_[i])) {
        if (!is_in_delim) {
          is_in_delim = true;
          result.push_back(String(*this, beg, i - beg));
          beg = i + 1;
        }
      } else {
        if (is_in_delim) {
          is_in_delim = false;
          beg = i;
        }
      }
    }
    if (beg <= end) result.push_back(String(*this, beg, end - beg + 1));
  }
  return std::move(result);
}

const TL::String TL::String::Substring(size_t start) const {
  return Substring(start, length_);
}

const TL::String TL::String::Substring(size_t start, size_t length) const {
  return std::move(String(*this, start, length));
}

const TL::String TL::String::Insert(size_t pos, wchar_t c, size_t n = 1) const {
  return String(*this).InsertM(pos, c, n);
}

const TL::String TL::String::Insert(size_t pos, const String& s) const {
  return String(*this).InsertM(pos, s);
}

const TL::String TL::String::Insert(size_t pos,
                                    const String& s,
                                    size_t start,
                                    size_t length) const {
  return String(*this).InsertM(pos, s, start, length);
}
  
const TL::String TL::String::Trim(
    const String& spaces = L" \t\r\n") const {
  return String(*this).TrimM(spaces);
}

const TL::String TL::String::TrimStart(
    const String& spaces = L" \t\r\n") const {
  return String(*this).TrimStartM(spaces);
}

const TL::String TL::String::TrimEnd(
    const String& spaces = L" \t\r\n") const {
  return String(*this).TrimEndM(spaces);
}

const TL::String TL::String::Remove(size_t start) const {
  return Substring(0, start);
}

const TL::String TL::String::Remove(size_t start, size_t length) const {
  return String(*this, 0, start) += 
      (length_ - start <= length) ? L"" : Substring(start + length);
}

const TL::String TL::String::Erase(wchar_t c) const {
  return String(*this).EraseM(c);
}

const TL::String TL::String::Erase(const String& s) const {
  return String(*this).EraseM(s);
}

const TL::String TL::String::EraseAny(const String& chars) const {
  return String(*this).EraseAnyM(chars);
}

const TL::String TL::String::EraseAll(wchar_t c) const {
  return String(*this).EraseAllM(c);
}

const TL::String TL::String::EraseAll(const String& s) const {
  return String(*this).EraseAllM(s);
}

const TL::String TL::String::EraseAnyAll(const String& chars) const {
  return String(*this).EraseAnyAllM(chars);
}

const TL::String TL::String::Replace(wchar_t old_char, wchar_t new_char) const {
  return String(*this).ReplaceM(old_char, new_char);
}

const TL::String TL::String::Replace(const String& old_str, 
                                     const String& new_str) const {
  return String(*this).ReplaceM(old_str, new_str);
}

const TL::String TL::String::ReplaceAll(wchar_t old_char, wchar_t new_char) const {
  return String(*this).ReplaceAllM(old_char, new_char);
}

const TL::String TL::String::ReplaceAll(const String& old_str,
                                        const String& new_str) const {
  return String(*this).ReplaceAllM(old_str, new_str);
}

const TL::String TL::String::Reverse() const {
  return String(rbegin(), rend());
}

const TL::String TL::String::Resize(size_t size, wchar_t c = L' ') const {
  return String(*this).ResizeM(size, c);
}

// in-place string algorithms, these algorithm will modify string itself
TL::String& TL::String::AssignM(const String& s, size_t start, size_t length) {
  if (start >= s.length()) throw std::out_of_range("String::AssignM()");
  length = std::min(s.length() - start, length);
  return AssignM(s.str_ + start, s.str_ + length);
}

template<typename Iter>
TL::String::AssignM(Iter beg, Iter end);

TL::String& TL::String::AppendM(wchar_t c) {
  return operator+=(c);
}

TL::String& TL::String::AppendM(const String& s) {
  return operator+=(s);
}

TL::String& TL::String::AppendM(const String& s,
                                size_t start,
                                size_t length) {
  
}
TL::String& TL::String::InsertM(size_t pos, wchar_t c, size_t n = 1) {
  return InsertM(pos, String(n, c));
}

TL::String& TL::String::InsertM(size_t pos, const String& s) {
  return InsertM(pos, s, 0, s.length());
}

TL::String& TL::String::InsertM(size_t pos,
                                const String& s,
                                size_t start,
                                size_t length) {
  if (pos >= length_) throw std::out_of_range("String::InsertM(): pos");
  if (start >= s.length()) throw std::out_of_range("String::InsertM(): start");
  if (kMaxLength - length_ < s.length()) throw std::length_error("String::InsertM()");
  length = std::min(s.length() - start, length);
  if (length_ + length < capacity_) {
    ShiftRight(str_ + pos, str_ + pos + length, length_ - pos);
    wcsncpy(str_ + pos, s.str() + start, length);
    length_ += length;
    str_[length_] = L'\0';
  } else {
    wchar_t* p = GrowLength(length_ + length);
    wcsncpy(p, str_, pos);
    wcsncpy(p + pos, s.str() + start, length);
    wcscpy(p + pos + length, str_ + pos);
    length_ += length;
    set_str(p);
  }
  return *this;
}

auto Find(int, int) -> int;

TL::String& TL::String::TrimM(const String& spaces = L" \t\r\n") {
  TrimEndM(spaces);
  TrimStartM(spaces);
  return *this;
}

TL::String& TL::String::TrimStartM(const String& spaces = L" \t\r\n") {
  size_t res = IndexOfNotAny(spaces);
  if (res == kNotFound) {
    ResizeM(0);
  } else {
    ShiftLeft(str_ + res, str_, length_ - res);
    ResizeM(length_ - res);
  }
  return *this;
}

TL::String& TL::String::TrimEndM(const String& spaces = L" \t\r\n") {
  size_t res = LastIndexOfNotAny(spaces);
  ResizeM(res == kNotFound ? 0 : res + 1);
  return *this;
}

TL::String& TL::String::RemoveM(size_t start) {
  if (start >= length_) throw std::out_of_range("String::RemoveM(size_t)");
  ResizeM(length_ - start);
  return *this;
}

TL::String& TL::String::RemoveM(size_t start, size_t length) {
  if (start >= length_) throw std::out_of_range("String::RemoveM(size_t, size_t)");
  length = std::min(length_ - start, length);
  ShiftLeft(str_ + start + length, str_ + start, length_ - start - length);
  length_ -= length;
  str_[length_] = L'\0';
  return *this;
}

TL::String& TL::String::EraseM(wchar_t c) {
  return EraseAnyM(String(1, c));
}

TL::String& TL::String::EraseM(const String& s) {
  size_t i = IndexOf(s);
  if (i != kNotFound) {

  }
  return *this;
}

String& EraseAnyM(const String& chars);
String& EraseAllM(wchar_t c);
String& EraseAllM(const String& s);
String& EraseAnyAllM(const String& chars);
String& ReplaceM(wchar_t old_char, wchar_t new_char);
String& ReplaceM(const String& old_str, const String& new_str);
String& ReplaceAllM(wchar_t old_char, wchar_t new_char);
String& ReplaceAllM(const String& old_str, const String& new_str);

TL::String& TL::String::ReverseM() {
  if (!IsEmpty())
    for (size_t beg = 0, end = length_ - 1; beg < end; ++beg, --end)
      std::swap(str_[beg], str_[end]);
  return *this;
}

TL::String& TL::String::ResizeM(size_t size, wchar_t c = L' ') {
  if (size <= length_) {
    str_[length_ = size] = L'\0';
  } else if (size < capacity_) {
    while (length_ < size)
      str_[length_++] = c;
    str_[length_] = L'\0';
  } else if (size <= kMaxLength) {
    wchar_t* p = new wchar_t[size + 1];
    wcscpy(p, str_);
    while (length_ < size)
      p[length_++] = c;
    p[length_] = L'\0';
    set_str(p);
    capacity_ = length_ + 1;
  } else {
    throw std::length_error("String::ResizeM(size_t, wchar_t)");
  }
  return *this;
}

TL::String& TL::String::ClearM() {
  return ResizeM(0);
}

// auxilirary
wchar_t* TL::String::AllocAndCopyFromStr(const wchar_t* pstr, size_t length) const {
  if (pstr != nullptr) {
    wchar_t* p = new wchar_t[length + 1];
    for (size_t i = 0; i < length; ++i)
      p[i] = pstr[i];
    p[length] = L'\0';
    return p;
  } else {
    throw TL::NullPointer("String::AllocAndCopyFromStr(const wchar_t*, size_t)");
  }
}

wchar_t* TL::String::GrowLength(size_t length) {
  // grow capacity to match new length
  if (capacity_ > length) return;
  if (length > kMaxLength) throw std::length_error("String::GrowLength(): length > kMaxLength");
  // be care of overflow!
  const static float kGrowFactor = 1.5;
  const static size_t kLimit = 2863311530;
  size_t new_cap =
    capacity_ >= kLimit ?
    kMaxLength :
    std::max(length, (size_t)(capacity_ * kGrowFactor)) + 1;
  wchar_t* p = new wchar_t[new_cap];
  capacity_ = new_cap;
  return p;
}

void TL::String::GuaranteeLength(size_t length) {
  // grow capacity to match new length
  if (capacity_ > length) return;
  if (length > kMaxLength) throw std::length_error("String::GuaranteeLength(): length > kMaxLength");
  // be care of overflow!
  const static float kGrowFactor = 1.5;
  const static size_t kLimit = 2863311530;
  size_t new_cap =
    capacity_ >= kLimit ?
    kMaxLength :
    std::max(length, (size_t)(capacity_ * kGrowFactor)) + 1;
  wchar_t* p = new wchar_t[new_cap];
  wcscpy(p, str_);
  set_str(p);
  capacity_ = new_cap;
  assert(length_ == wcslen(str_));
}

void TL::String::ShiftLeft(const wchar_t* source, wchar_t* dest, size_t length) {
  if (source != dest && *source)
    for (size_t i = 0; i < length; ++i)
      dest[i] = source[i];
}

void TL::String::ShiftRight(const wchar_t* source, wchar_t* dest, size_t length) {
  if (source != dest && *source)
    for (size_t i = length - 1; i >= 0; --i)
      dest[i] = source[i];
}

void TL::String::set_str(wchar_t* p) {
  delete[] str_;
  str_ = p;
}
