#pragma once

#include "ibulklogger.h"
#include "ifilelogger.h"

namespace otus {
	/**
	 * File logger, that do nothing. Used in tests.
	 */
	class EmptyFileLogger: public IFileLogger {
	  void write_to_file(const time_point_t &, const std::vector<std::string> &text_results) override;
	};

	/**
	 * File logger, print bulk in file with name first_command_timestamp.log
	 */
	class FileLogger: public IFileLogger {
	public:
	  void write_to_file(const time_point_t &time_point,
						 const std::vector<std::string> &text_results) override;
	};

	/**
	 * Class for beautiful printing input command and output bulk of them after processing these commands.
	 */
	class BulkLogger: public IBulkLogger {
	public:
		explicit BulkLogger(std::ostream &ostream);
	  void log_output(const std::vector<std::string> &text_results) override;

	private:
		std::ostream &ostream_;
	};
} // namespace otus
