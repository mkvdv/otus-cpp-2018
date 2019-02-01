#pragma once

#include <string>
#include <vector>

namespace otus {
  class IBulkLogger {
   public:
    virtual void log_output(const std::vector<std::string> &text_results) = 0;
    virtual ~IBulkLogger() = default;
  };
} // namespace otus
