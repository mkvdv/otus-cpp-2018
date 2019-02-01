#pragma once

#include <chrono>
#include <string>
#include <vector>

namespace otus {
  class IFileLogger {
   public:
    using time_point_t = std::chrono::time_point<std::chrono::system_clock>;

    virtual void write_to_file(const time_point_t &time_point,
                               const std::vector<std::string> &text_results) = 0;
    virtual ~IFileLogger() = default;
  };
} // namespace otus
