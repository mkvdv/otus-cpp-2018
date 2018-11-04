#include "logger_job.h"
#include "../utils/counter.h"

#include <string>
#include <ostream>


namespace otus {
	LoggerJob::LoggerJob(std::ostream &stream,
	                     std::string text,
	                     const StatCounter &counter,
	                     std::string prefix)
		: stream_(stream), text_(std::move(text)), counter_(counter), prefix_(std::move(prefix)) {
	}

	StatCounter LoggerJob::run() {
		stream_ << prefix_ << text_ << "\n";
		return counter_;
	}
} // namespace otus
