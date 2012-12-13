#ifndef TL_ARRAY_H_
#define TL_ARRAY_H_

namespace TL {

template <typename T>
class Array {
 public:
  Array();
  explicit Array(size_t n);
  Array(size_t n, const T& value);
  ~Array();

  Array<T>& operator=(const Array<T>& rhs);
  T& operator[](size_t index);
  const T& operator[](size_t index) const;

  void Add(const T& value);
  void Remove(const T& value);
  void RemoveAll(const T& value);
  void Clear();

  bool IsEmpty() const;
  bool IsContains() const;

  // Iterator<T> begin() const;
  // Iterator<T> end() const;
  size_t size() const;
  size_t capacity() const;
 private:
  size_t size_, capacity_;
  T* data_;
};

} // namespace TL

#endif // TL_ARRAY_H_
