#include "logger.h"

#include <fstream>

namespace otus {
	void FileLogger::write_to_file(const FileLogger::time_point_t &time_point, const std::string &text) {
		std::string path = std::to_string(time_point.time_since_epoch().count()) + ".log";

		std::ofstream fs(path);
		fs << text << std::endl;
	}

	void EmptyFileLogger::write_to_file(const IFileLogger::time_point_t &, const std::string &) {
		/**
		 * Do nothing.
		 */
	}

	BulkLogger::BulkLogger(std::ostream &ostream)
		: ostream_(ostream) {}

	void BulkLogger::log_input(const std::string &cmd_string) {
		ostream_ << cmd_string << "\t|\n";
	}

	void BulkLogger::log_output(const std::string &bulk_message) {
		ostream_ << "\t| " << bulk_message << '\n';
	}
} // namespace otus
