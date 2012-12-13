#ifndef TL_PTR_H_
#define TL_PTR_H_

namespace TL {

template <typename T>
class Ptr { // share pointer
 public:
  Ptr() : count_(nullptr), ptr_(nullptr) {}
  Ptr(const T* const p) : count_(new int(1)), ptr_(p) {}
  Ptr(const Ptr& ptr) : count_(ptr.count_), ptr_(ptr.ptr_) { ++count_; }
  ~Ptr();
  bool operator() const { return ptr_ == nullptr; }
  T operator*() { return *ptr_; }
  const T* ptr() const { return ptr_; }
 private:
  int* count_;
  T* ptr_; 
}; // class Ptr

} // namespace TL

#endif // TL_PTR_H_
