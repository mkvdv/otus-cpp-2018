#include "exceptions.h"

namespace otus {
  namespace exception {
    terminate_exception::terminate_exception(const char *what_arg)
        : runtime_error(what_arg) {}
  } // namespace exception
} // namespace exception
