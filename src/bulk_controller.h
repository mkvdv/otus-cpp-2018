#pragma once

#include "command/icommand_pool.h"
#include "io/ibulklogger.h"
#include "io/ifilelogger.h"
#include "io/ireader.h"

namespace otus {
	/**
	 * BulkController use bunch loggers, reader and command_pool
	 * for reading, executing commands and logging bulks in file and other streams.
	 *
	 * It use 2 loggers
	 * - IFileLogger - write bulk of commands in special .log files
	 * - IBulkLogger - write the same bulks to output and also it can write input data in pretty way
	 */
	class BulkController: public Listener<std::string> {
	public:
		/**
		 *
		 * @param commands_per_block - size of block in commands
		 * @param print_input_enabled - if true, than all input will be also printed to output in pretty
		 * way, it is useful if u redirect stdin from file and can't see real input
		 */
		explicit BulkController(size_t commands_per_block,
		                        std::unique_ptr<IFileLogger> file_logger,
		                        std::unique_ptr<IReader> reader,
		                        std::unique_ptr<IBulkLogger> bulk_logger,
		                        std::unique_ptr<ICommandPool> command_pool,
		                        bool print_input_enabled = false);

		/*
		 * Start point of command reading and executing.
		 * Launch reader and react after reader get new string.
		 */
		void start();

		/**
		 * Realization of Listener pattern - react for new string from Reader.
		 * @param s
		 */
		void update(const std::string &s) override;

	private:
		/**
		 * Run commands in poll and write output using loggers.
		 */
		void run_all_commands();

		size_t commands_per_block_;
		size_t depth_ = 0;

		std::unique_ptr<IFileLogger> file_logger_;
		std::unique_ptr<IReader> reader_;
		std::unique_ptr<IBulkLogger> bulk_logger_;
		std::unique_ptr<ICommandPool> pool_;
		bool print_input_enabled_;
	};
} // namespace otus
