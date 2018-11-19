#pragma once

#include <exception>

namespace otus::exception {
  class terminate_exception : public std::exception {
	  const char *what() const noexcept override;
  };
} // namespace otus::exception
