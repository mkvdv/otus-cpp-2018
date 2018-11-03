#pragma once

#include <atomic>

#include "../io/ireader.h"
#include "../active/threadsafe_queue.h"

namespace otus::async {
	/**
	 * Special IReader implementation, that use threadsafe queue instead of
	 * std::istream as reading source.
	 */
	class ThreadsafeReader: public IReader {
	public:
		using queue_t = ThreadsafeQueue<std::string>;

		explicit ThreadsafeReader(queue_t &);
		void start_reading_cycle() override;
	private:
		queue_t &queue_of_lines_;
		std::atomic<bool> stopped_;
	};
} // namespace otus::async
