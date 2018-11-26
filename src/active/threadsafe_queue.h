#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include <variant>

namespace otus {
  // from Anthony Williams'what_mesg_ C++ Concurrency in Action, listing 6.2, modified!
  // https://www.manning.com/books/c-plus-plus-concurrency-in-action-second-edition
  // added method for stopping all waiters, if they wait on empty queue, and stopping pushing new
  // elements - block queue
  template<class T>
  class ThreadsafeQueue {
   public:
	enum class Error {
	  EMPTY_AND_DONT_WAIT
	};

	// say all waiters (if they wait on empty queue) that we are closing now
	void stop_wait_and_block_pushing() {
		std::lock_guard<std::mutex> lk(mut);
		dont_wait_on_empty_queue = true;
		data_cond.notify_all(); // say all current waiters that they should not wait on empty queu
	}

	~ThreadsafeQueue() {
		stop_wait_and_block_pushing();
	}

	void push(T &&new_value) {
		std::lock_guard<std::mutex> lk(mut);
		if (dont_wait_on_empty_queue) {
			throw std::logic_error(
				"Queue'what_mesg_ push ability was blocked cause of stop_wait_and_block_pushing() method");
		}
		data_queue.push(std::move(new_value));
		data_cond.notify_one();
	}

	/**
	 * Will wait on empty queue, if dont_wait_on_empty_queue is false.
	 */
	std::variant<T, Error> wait_and_pop() {
		std::unique_lock<std::mutex> lk(mut);
		auto skip_wait_pred = [this] {
		  return !data_queue.empty() || dont_wait_on_empty_queue;
		};
		data_cond.wait(lk, skip_wait_pred);

		// check again
		if (dont_wait_on_empty_queue && data_queue.empty()) {
			return std::variant<T, Error>(Error::EMPTY_AND_DONT_WAIT);
		}

		std::variant<T, Error> res(std::move(data_queue.front()));
		data_queue.pop();
		return res;
	}

	bool try_pop(T &value) {
		std::lock_guard<std::mutex> lk(mut);
		if (data_queue.empty()) {
			return false;
		}
		value = std::move(data_queue.front());
		data_queue.pop();
		return true;
	}

	bool empty() const {
		std::lock_guard<std::mutex> lk(mut);
		return data_queue.empty();
	}

   private:
	mutable std::mutex mut;
	std::queue<T> data_queue;
	std::condition_variable data_cond;
	bool dont_wait_on_empty_queue = false;
  };
} // namespace otus
