#ifndef TL_RANGE_H_
#define TL_RANGE_H_

namespace TL {

// template<typename T>
// 特化 Range<size_t>, Range<int>
// 先特化, 在泛化
class Range {
 public:
  Range(int end) : start_(0), end_(end), step_(1), current_(0), is_used_(false) {}
  Range(int start, int end) :
      start_(start_), end_(end), step_(1), current_(start), is_used_(false) {}
  Range(int start, int end, int step) :
      start_(start), end_(end), step_(step), current_(start), is_used_(false) {}
  void Reset() { current_ = start_; is_used_ = false; }
 private:
  Range(const Range&);
  Range& operator=(const Range&);
  const int start_, end_, step_;
  int current_;
  bool is_used_;
}; // class Range

template<typename T>
Range<T> MakeRange(T end) {
  return Range<T>(end);
}

template<typename T> 
Range<T> MakeRange(T start, T end) {
  return Range<T>(start, end);
}

template<typename T> 
Range<T> MakeRange(T start, T end, T step) {
  return Range<T>(start, end, step);
}

} // namespace TL

#endif // TL_RANGE_H_
