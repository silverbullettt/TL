#ifndef TL_STRING_H_
#define TL_STRING_H_

#include <vector>

namespace TL {

// template<typename T> class Array;

class String {
 public:
  typedef wchar_t* iterator;
  typedef const wchar_t* const_iterator;
  // static constants
  const static size_t kNotFound, kMaxLength;
  
  // constructors & destructor
  String();
  explicit String(size_t n);
  String(size_t n, wchar_t c);
  String(const wchar_t* p);
  String(const String& other);
  String(String&& other);
  String(const String& s, size_t start, size_t n);
  template<typename Iter> String(Iter beg, Iter end);
  // 如何处理好 Iterator 与 const 之间的关系？
  ~String();

  // overload operators
  String& operator=(const String& other);
  String& operator=(String&& other);
  String& operator+=(wchar_t c);
  String& operator+=(const String& s);
  wchar_t& operator[](size_t index);
  const wchar_t& operator[](size_t index) const;

  void Swap(String& s);
  // return new String(s)
  // const Array<String> Split(wchar_t delim) const;
  // const Array<String> Split(const String& delims) const;
  const std::vector<String> Split(wchar_t delim) const;
  const std::vector<String> Split(const String& delims) const;
  const String Substring(size_t start) const;
  const String Substring(size_t start, size_t length) const;
  const String Insert(size_t pos, wchar_t c, size_t n = 1) const;
  const String Insert(size_t pos, const String& s) const;
  const String Insert(size_t pos, const String& s, size_t start, size_t length) const;
  const String Trim(const String& spaces = L" \t\r\n") const;
  const String TrimStart(const String& spaces = L" \t\r\n") const;
  const String TrimEnd(const String& spaces = L" \t\r\n") const;

  const String Remove(size_t start) const;
  const String Remove(size_t start, size_t length) const;
  const String Erase(wchar_t c) const;
  const String Erase(const String& s) const;
  const String EraseAny(const String& chars) const;
  const String EraseAll(wchar_t c) const;
  const String EraseAll(const String& s) const;
  const String EraseAnyAll(const String& chars) const;
  const String Replace(wchar_t old_char, wchar_t new_char) const;
  const String Replace(const String& old_str, const String& new_str) const;
  const String ReplaceAll(wchar_t old_char, wchar_t new_char) const;
  const String ReplaceAll(const String& old_str, const String& new_str) const;

  const String Reverse() const;
  const String Resize(size_t size, wchar_t c = L' ') const;
  // in-place algorithms, modify itself
  String& AssignM(const String& s, size_t start, size_t length);
  template<typename Iter> AssignM(Iter beg, Iter end);
  String& AppendM(wchar_t c);
  String& AppendM(const String& s);
  String& AppendM(const String& s, size_t start, size_t length);
  String& InsertM(size_t pos, wchar_t c, size_t n = 1);
  String& InsertM(size_t pos, const String& s);
  String& InsertM(size_t pos, const String& s, size_t start, size_t length);
  String& JoinM(const String& s);
  // template<typename Iter> String& JoinM(Iter beg, Iter end);
  String& TrimM(const String& spaces = L" \t\r\n");
  String& TrimStartM(const String& spaces = L" \t\r\n");
  String& TrimEndM(const String& spaces = L" \t\r\n");

  String& RemoveM(size_t start);
  String& RemoveM(size_t start, size_t length);
  String& EraseM(wchar_t c);
  String& EraseM(const String& s);
  String& EraseAnyM(const String& chars);
  String& EraseAllM(wchar_t c);
  String& EraseAllM(const String& s);
  String& EraseAnyAllM(const String& chars);
  String& ReplaceM(size_t pos, size_t length, const String& s);
  String& ReplaceM(size_t pos, size_t length, const String& s, size_t start, size_t n);
  String& ReplaceM(wchar_t old_char, wchar_t new_char);
  String& ReplaceM(const String& old_str, const String& new_str);
  String& ReplaceAllM(wchar_t old_char, wchar_t new_char);
  String& ReplaceAllM(const String& old_str, const String& new_str);

  String& ReverseM();
  String& ResizeM(size_t size, wchar_t c = L' ');
  String& ClearM();

  // search index
  size_t IndexOf(wchar_t c) const;
  size_t IndexOfAny(const String& chars) const;
  size_t IndexOfNot(wchar_t c) const;
  size_t IndexOfNotAny(const String& chars) const;
  size_t IndexOf(const String& s) const;
  size_t IndexOf(const String& s, size_t start, size_t n) const;

  size_t LastIndexOf(wchar_t c) const;
  size_t LastIndexOfAny(const String& chars) const;
  size_t LastIndexOfNot(wchar_t c) const;
  size_t LastIndexOfNotAny(const String& chars) const;
  size_t LastIndexOf(const String& s) const;
  size_t LastIndexOf(const String& s, size_t start, size_t n) const;

  // judge content
  bool IsEmpty() const { return length_ == 0; }
  bool IsStartsWith(const String& s) const;
  bool IsEndsWith(const String& s) const;
  bool IsContains(wchar_t c) const;
  bool IsContains(const String& s) const;
  bool IsContainsAny(const String& chars) const;

  // getter
  const wchar_t* str() const { return str_; }
  size_t length() const { return length_; }
  size_t size() const { return length_; }
  // iterator begin() const;
  // iterator end() const;

 private:
  wchar_t* AllocAndCopyFromStr(const wchar_t* pstr, size_t length) const;
  wchar_t* GrowLength(size_t length);
  void GuaranteeLength(size_t length);
  void ShiftLeft(const wchar_t* source, wchar_t* dest, size_t length);
  void ShiftRight(const wchar_t* source, wchar_t* dest, size_t length);
  // take out?
  size_t SundayFind(const String& s) const;
  std::vector<size_t> SundayFindAll(const String& s) const;
  size_t SundayReverseFind(const String& s) const;
  std::vector<size_t> SundayReverseFindAll(const String& s) const;

  void set_str(wchar_t* p);

  size_t length_, capacity_;
  wchar_t* str_;
}; // class String

const String operator+(const String& lhs, const String& rhs);
const String operator+(const String& s, wchar_t c);
bool operator==(const String& lhs, const String& rhs);
bool operator!=(const String& lhs, const String& rhs);
bool operator<(const String& lhs, const String& rhs);
bool operator>(const String& lhs, const String& rhs);
bool operator<=(const String& lhs, const String& rhs);
bool operator>=(const String& lhs, const String& rhs);

/*
template <typename T>
String Format(const String& s, T t);
template <typename T0, typename T1>
String Format(const String& s, T0 t0, T1 t1);
template <typename T0, typename T1, typename T2>
String Format(const String& s, T0 t0, T1 t1, T2, t2);
*/

} // namespace TL

#endif // TL_STRING_H_
