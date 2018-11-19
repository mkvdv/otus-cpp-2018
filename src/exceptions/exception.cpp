#include "exception.h"

namespace otus::exception {
  const char *terminate_exception::what() const noexcept {
	  return "Program will be terminated!";
  }
}
