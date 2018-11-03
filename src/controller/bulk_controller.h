#pragma once

#include "../command/icommand_pool.h"
#include "../io/ireader.h"
#include "../io/logger_job.h"
#include "../io/file_logger_job.h"
#include "../active/threadsafe_queue.h"
#include "../utils/counter.h"

#include <string>
#include <memory>

namespace otus {
	/**
	 * BulkController use reader and command_pool
	 * for reading, executing commands.
	 *
	 * It use 2 logger queues - create jobs for loggers, that created outside this class.
	 */
	class BulkController
		: public Listener<std::string>, public std::enable_shared_from_this<BulkController> {
	public:
		/**
		 *
		 * @param commands_per_block - size of block in commands
		 * @param print_input_enabled - if true, than all input will be also printed to output in pretty
		 * way, it is useful if u redirect stdin from file and can't see real input
		 */
		BulkController(size_t commands_per_block,
		               std::unique_ptr<IReader> reader,
		               std::unique_ptr<ICommandPool> command_pool,
		               otus::ThreadsafeQueue<LoggerJob> &logger_jobs,
		               otus::ThreadsafeQueue<FileLoggerJob> &file_logger_jobs,
		               std::ostream &logger_ostream,
		               std::ostream &stats_stream);
		~BulkController() override;

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

	private:
		size_t commands_per_block_;

		std::unique_ptr<IReader> reader_;
		std::unique_ptr<ICommandPool> pool_;

		otus::ThreadsafeQueue<LoggerJob> &logger_jobs_;
		otus::ThreadsafeQueue<FileLoggerJob> &file_logger_jobs_;
		std::ostream &logger_ostream_;
		std::ostream &stats_stream_;

		size_t depth_ = 0;
		StatCounter current_counter_{};
		StatCounter prev_jobstart_counter_{};
	};
} // namespace otus
