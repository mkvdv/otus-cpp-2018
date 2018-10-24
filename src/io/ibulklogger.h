#pragma once

#include <string>

namespace otus {
	class IBulkLogger {
	public:
		virtual void log_output(const std::string &bulk_message) = 0;
		virtual ~IBulkLogger() = default;
	};
} // namespace otus
