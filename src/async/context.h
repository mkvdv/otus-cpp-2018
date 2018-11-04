#pragma once

#include "../active/threadsafe_queue.h"
#include "../active/job_pool.h"
#include "../io/file_logger_job.h"
#include "../io/ireader.h"
#include "../io/logger_job.h"
#include "../command/icommand_pool.h"
#include "../controller/bulk_controller.h"

#include <memory>
#include <sstream>
#include <string>
#include <list>

namespace otus::async {
	/**
	 * Execution context for async connectons.
	 * Manage all entities from bulk task.
	 *
	 * About threads: context create
	 * 	- 1 worker thread for BulkController working cycle
	 * 		- which create 1 logger thread
	 * 		- and FILELOGGER_THREADS_COUNT filelogger threads
	 * So, be carefully, if u create lots of contexts.
	 * CAUTION: no threadpool in use here, so lots of Contexts can be bad for performance cause
	 * synchronization
	 */
	class Context {
	public:
		using ActiveLogger = otus::JobPool<otus::LoggerJob, 1>;

		explicit Context(size_t bulk_size, const std::string &prefix);

		Context(Context &&) = delete;
		Context &operator=(Context &&) = delete;

		// noncopyable
		Context(const Context &) = delete;
		Context &operator=(const Context &) = delete;

		void input(const std::string &);
		void block_queue_input_and_stop_waiters();
		size_t get_bulk_size() const;

		~Context();

		friend inline bool operator<(const Context &, const Context &);

	private:
		static const size_t FILELOGGER_THREADS_COUNT = 2;
		std::pair<std::list<std::string>, std::string> split(std::string &&);

		size_t bulk_size_;

		otus::ThreadsafeQueue<otus::LoggerJob> logger_jobs_{};
		ActiveLogger active_logger_;
		otus::ThreadsafeQueue<otus::FileLoggerJob> file_logger_jobs_{};
		otus::JobPool<otus::FileLoggerJob, FILELOGGER_THREADS_COUNT> file_logger_pool_;

		std::unique_ptr<otus::IReader> reader_;
		std::unique_ptr<otus::ICommandPool> command_pool_;
		std::shared_ptr<otus::BulkController> controller_;

		ThreadsafeQueue<std::string> queue_of_input_lines_{};
		std::string previosly_lost_str_{};

		std::thread worker_;
	};

	inline bool operator<(const Context &lhs, const Context &rhs) {
		return lhs.bulk_size_ < rhs.bulk_size_;
	}

} // namespace otus
