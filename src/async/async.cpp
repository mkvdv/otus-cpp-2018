#include "async.h"
#include "context.h"

#include <map>
#include <mutex>

// it's real to do it today !!!!

namespace otus::async {
	namespace {
		std::map<Context *, std::unique_ptr<Context>> g_contexts_;

		std::mutex mt;
	} // anonymous namespace


	handle_t connect(std::size_t bulk) {
		std::unique_ptr<Context> ptr = std::make_unique<Context>(bulk);
		Context *raw_ptr = ptr.get();

		std::unique_lock lock(mt);
		auto p = g_contexts_.emplace(std::make_pair(raw_ptr, std::move(ptr)));
		lock.unlock();

		if (p.second) {
			return reinterpret_cast<handle_t>(raw_ptr);
		} else {
			throw std::runtime_error("Can't create new context");
		}
	}

	void receive(handle_t handle, const char *data, std::size_t size) {
		std::unique_lock lock(mt);

		auto it = g_contexts_.find(reinterpret_cast<Context * >(handle));
		if (it != g_contexts_.end()) {
			lock.unlock();
			it->second->input(std::string(data, size));
		} else {
			throw std::runtime_error("Invalid handle, not created or already deleted");
		}
	}

	void disconnect(handle_t handle) {
		std::unique_lock lock(mt);

		auto it = g_contexts_.find(reinterpret_cast<Context * >(handle));
		if (it != g_contexts_.end()) {
			lock.unlock();

			// can be long destroy process (join many threads), so unlock
			it->second->block_queue_input_and_stop_waiters();
			it->second.reset();

			lock.lock();
			g_contexts_.erase(it);
		} else {
			throw std::runtime_error("Invalid handle, not created or already deleted");
		}
	}
} // namespace otus::async
