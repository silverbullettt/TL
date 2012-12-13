#ifndef TL_EXCEPTION_H_
#define TL_EXCEPTION_H_

#include <stdexcept>
#include <string>

namespace TL {

class NullPointer : public std::invalid_argument {
 public:
  explicit NullPointer(const std::string& msg) : std::invalid_argument(msg) {}
};


} // namespace TL

#endif // TL_EXCEPTION_H_
