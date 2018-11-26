#pragma once

#include "threadsafe_queue.h"

#include <thread>
#include <condition_variable>
#include <atomic>
#include <iostream>

#include <boost/log/trivial.hpp>

namespace otus {
  /**
   * Read from task_queue while it is not stopped and queue is not empty, or while queue don't say
   * to us not to wait new elements using EMPTY_AND_DONT_WAIT error code.
   * After getting new JobType job it execute it, calling it's run() method.
   */
  template<class JobType, size_t MAX_THREADS>
  class JobPool {
	public:
	explicit JobPool(std::shared_ptr<otus::ThreadsafeQueue<JobType>> task_queue) : job_queue_(std::move(
		task_queue)) {
		for (size_t ix = 0; ix != MAX_THREADS; ++ix) {
			workers_[ix] = std::move(std::thread(&JobPool::worker, this));
		}
	}

	~JobPool() {
		done_ = true;
		join_all();
	}

	void join_all() {
		for (std::thread &t: workers_) {
			if (t.joinable()) {
				t.join();
			}
		}
	}

	private:
	void worker() {
		while (!done_ || !job_queue_->empty()) {
			auto job_or_error = job_queue_->wait_and_pop();
			BOOST_LOG_TRIVIAL(info) << this << " got something from queue";

			try {
				std::get<JobType>(job_or_error).run();
			}
			catch (const std::bad_variant_access &) {
				auto err = std::get<queue_error_t>(job_or_error);
				if (err == queue_error_t::EMPTY_AND_DONT_WAIT) {
					BOOST_LOG_TRIVIAL(debug) << this << " queue empty and waiting on it is not allowed";
					return;
				} else {
					std::cerr << "unexpected error type" << std::endl;
				}
			}
		}
	}

	private:
	using queue_error_t = typename ThreadsafeQueue<JobType>::Error;
	std::shared_ptr<otus::ThreadsafeQueue<JobType>> job_queue_;
	std::array<std::thread, MAX_THREADS> workers_;
	std::atomic<bool> done_{false};
  };
} // namespace otus
