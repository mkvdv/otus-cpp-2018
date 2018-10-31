#pragma once

#include "threadsafe_queue.h"
#include "../io/logger_job.h"

#include <thread>
#include <atomic>
#include <iostream>

namespace otus {
	/**
	 * Read from logging_queue while it is not stopped or queue empty and say to us not to wait new elements.
	 * After getting new JobType job it execute it, calling it's run() method.
	 */
	template<class JobType>
	class ActiveLogger {
	public:
		explicit ActiveLogger(ThreadsafeQueue<JobType> &logging_queue, std::ostream &stats_stream)
			: logging_queue_(logging_queue),
			  stats_stream_(stats_stream),
			  worker_(std::thread(&ActiveLogger::worker_thread, this)) {
		}

		ActiveLogger(const ActiveLogger &) = delete;
		ActiveLogger &operator=(const ActiveLogger &) = delete;

		void stop() {
			stopped_ = true;
		}

		void join() {
			if (worker_.joinable()) {
				worker_.join();
			}
		}

		~ActiveLogger() {
			stop();
			join();
			stats_stream_ << "Stats for ActiveLogger:\t\t" << counter_ << std::endl;
		}

	private:
		void worker_thread() {
			while (!stopped_) {
				auto job_or_error = logging_queue_.wait_and_pop();

				try {
					StatCounter job_counter = std::get<JobType>(job_or_error).run();
					counter_ += job_counter;
				}
				catch (const std::bad_variant_access &) {
					auto err = std::get<queue_error_t>(job_or_error);
					if (err == queue_error_t::EMPTY_AND_DONT_WAIT) {
						return;
					} else {
						std::cerr << "unexpected error type, ActiveLogger stops it's job" << std::endl;
						// no exception cause we don't catch exceptions from worker thread
					}
				}
			}
		}

	private:
		using queue_error_t = typename ThreadsafeQueue<JobType>::Error;
		ThreadsafeQueue<JobType> &logging_queue_;
		std::ostream &stats_stream_;
		std::thread worker_;
		std::atomic<bool> stopped_{false};
		StatCounter counter_{};
	};

} // namespace otus
