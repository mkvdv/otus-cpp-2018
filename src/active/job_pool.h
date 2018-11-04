#pragma once

#include "threadsafe_queue.h"
#include "../utils/counter.h"

#include <thread>
#include <condition_variable>
#include <atomic>
#include <iostream>


namespace otus {
	/**
	 * Read from task_queue while it is not stopped and queue is not empty, or while queue don'tsay
	 * to us not to wait new elements using EMPTY_AND_DONT_WAIT error code.
	 * After getting new JobType job it execute it, calling it's run() method.
	 */
	template<class JobType, size_t MAX_THREADS>
	class JobPool {
	public:
		JobPool(otus::ThreadsafeQueue<JobType> &task_queue,
		        std::ostream &stats_stream, std::string &&thread_group_name, std::string prefix = "")
			: job_queue_(task_queue),
			  stats_stream_(stats_stream),
			  thread_group_name_(thread_group_name),
			  prefix_(std::move(prefix)) {
			for (size_t ix = 0; ix != MAX_THREADS; ++ix) {
				workers_[ix] = std::move(std::thread(&JobPool::worker, this, ix));
			}
		}

		~JobPool() {
			done_ = true;
			join_all();

			// Print Counter for all threads
			for (size_t i = 0; i != counters_.size(); ++i) {
				stats_stream_ << prefix_ << "Stats for " << thread_group_name_ << i << "\tfrom JobPool:\t"
				              << counters_[i] << std::endl;
			}
		}

		void join_all() {
			for (std::thread &t: workers_) {
				if (t.joinable()) {
					t.join();
				}
			}
		}

	private:
		void worker(size_t thread_ix) {
			while (!done_ || !job_queue_.empty()) {
				auto job_or_error = job_queue_.wait_and_pop();

				try {
					StatCounter job_counter = std::get<JobType>(job_or_error).run();
					counters_[thread_ix] += job_counter;
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
		ThreadsafeQueue<JobType> &job_queue_;
		std::ostream &stats_stream_;
		std::string thread_group_name_;
		std::string prefix_;
		std::array<std::thread, MAX_THREADS> workers_;
		std::atomic<bool> done_{false};
		std::array<StatCounter, MAX_THREADS> counters_{};
	};
} // namespace otus
