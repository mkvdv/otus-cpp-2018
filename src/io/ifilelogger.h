#pragma once

#include <chrono>
#include <string>

namespace otus {
	class IFileLogger {
	public:
		using time_point_t = std::chrono::time_point<std::chrono::system_clock>;

		virtual void write_to_file(const time_point_t &time_point, const std::string &text) = 0;
		virtual ~IFileLogger() = default;
	};
} // namespace otus
