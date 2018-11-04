#pragma once

#include <chrono>
#include <string>
#include "../utils/counter.h"

namespace otus {
	class LoggerJob {
	public:
		LoggerJob(std::ostream &stream,
		          std::string text,
		          const StatCounter &counter,
		          std::string prefix = "");

		LoggerJob(const LoggerJob &) = delete;
		LoggerJob &operator=(const LoggerJob &) = delete;
		LoggerJob &operator=(LoggerJob &&) = delete;

		LoggerJob(LoggerJob &&) = default;

		StatCounter run();

	private:
		std::ostream &stream_;
		std::string text_;
		StatCounter counter_;
		std::string prefix_;
	};

} // namespace otus
