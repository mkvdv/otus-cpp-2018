#include "file_logger_job.h"

#include <string>
#include <fstream>
#include <iostream>
#include <thread>
#include <sstream>

namespace otus {
	FileLoggerJob::FileLoggerJob(std::string text, time_point_t time_point, const StatCounter &counter)
		: text_(std::move(text)), time_point_(time_point), counter_(counter) {
	}

	StatCounter FileLoggerJob::run() {
		std::stringstream path;
		path << std::to_string(time_point_.time_since_epoch().count()) << "_" << std::this_thread::get_id()
		     << ".log";

		std::ofstream fs(path.str());
		if (fs) {
			fs << text_ << '\n';
			fs.close();
		} else {
			std::cerr << "CANT OPEN FILE FOR WRITING" << std::endl;
		}

		return counter_;
	}
} // namespace otus
