#pragma once

#include <stdexcept>

namespace otus {
  namespace exception {
	class terminate_exception : public std::runtime_error {
	 public:
	  explicit terminate_exception(const char *what_arg) : runtime_error(what_arg) {}
	};
  } // namespace exception
} // namespace exception
