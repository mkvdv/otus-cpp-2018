#include "exceptions.h"

namespace otus::exception {
  terminate_exception::terminate_exception(const char *what_arg)
      : runtime_error(what_arg) {}
} // namespace otus::exception
