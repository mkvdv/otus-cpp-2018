#include "async.h"
#include "context.h"

#include <map>
#include <mutex>

// it's real to do it today !!!!

namespace otus::async {
	namespace {
		std::map<uint64_t, std::unique_ptr<Context>> g_contexts_;

		uint64_t context_counter = 0;

		std::mutex mt;
	} // anonymous namespace


	handle_t connect(std::size_t bulk) {
		std::lock_guard lock_guard(mt);
		++context_counter;

		auto p = g_contexts_.emplace(context_counter,
		                             std::make_unique<Context>(bulk,
		                                                       std::string{"["}
			                                                       + std::to_string(context_counter)
			                                                       + "] "));
		if (p.second) {
			return reinterpret_cast<handle_t>(context_counter);
		} else {
			--context_counter;
			throw std::runtime_error("Can't create new context");
		}
	}

	void receive(handle_t handle, const char *data, std::size_t size) {
		std::lock_guard lock_guard(mt);

		auto it = g_contexts_.find(reinterpret_cast<size_t >(handle));
		if (it != g_contexts_.end()) {
			it->second->input(std::string(data, size));
		} else {
			throw std::runtime_error("Invalid handle, not created or already deleted");
		}
	}

	void disconnect(handle_t handle) {
		std::lock_guard lock_guard(mt);

		auto it = g_contexts_.find(reinterpret_cast<size_t >(handle));
		if (it != g_contexts_.end()) {
			it->second->block_queue_input_and_stop_waiters();
			g_contexts_.erase(it);
		} else {
			throw std::runtime_error("Invalid handle, not created or already deleted");
		}
	}
} // namespace otus::async
