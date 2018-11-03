#include <iostream>
#include "threadsafe_reader.h"

namespace otus::async {
	ThreadsafeReader::ThreadsafeReader(ThreadsafeQueue<std::string> &queue)
		: queue_of_lines_(queue) {}

	void ThreadsafeReader::start_reading_cycle() {
		while (!stopped_) {
			std::variant<std::string, queue_t::Error> res = queue_of_lines_.wait_and_pop();
			std::string tmp;

			try {
				tmp = std::get<std::string>(res);
			}
			catch (const std::bad_variant_access &) {
				auto err = std::get<queue_t::Error>(res);
				if (err == queue_t::Error::EMPTY_AND_DONT_WAIT) {
					return;
				} else {
					std::cerr << "unexpected error type, ThreadsafeReader stops it's reading" << std::endl;
					// no exception cause we don't catch exceptions from worker thread
				}
			}

			if (!tmp.empty()) {
				notify_all(tmp); // notifi listener -- some BulkController
			}
		}
	}
} // namespace otus::async
