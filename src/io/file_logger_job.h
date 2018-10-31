#pragma once

#include <chrono>
#include <string>
#include "../utils/counter.h"

namespace otus {
	namespace {
		using time_point_t = std::chrono::time_point<std::chrono::system_clock>;
	}

	class FileLoggerJob {
	public:
		FileLoggerJob(std::string text, time_point_t time_point, const StatCounter &counter);

		FileLoggerJob(const FileLoggerJob &) = delete;
		FileLoggerJob &operator=(const FileLoggerJob &) = delete;
		FileLoggerJob &operator=(FileLoggerJob &&) = delete;

		FileLoggerJob(FileLoggerJob &&) = default;

		StatCounter run();

	private:
		std::string text_;
		time_point_t time_point_;
		StatCounter counter_;
	};
} // namespace otus
