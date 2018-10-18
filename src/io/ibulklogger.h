#pragma once

#include <string>

namespace otus {
	class IBulkLogger {
	public:
		virtual void log_input(const std::string &cmd_string) = 0;
		virtual void log_output(const std::string &bulk_message) = 0;
		virtual ~IBulkLogger() = default;
	};
} // namespace otus
