#include "async.h"
#include "context.h"

#include <map>
#include <mutex>

// it's real to do it today !!!!

namespace otus::async {
	namespace {
		std::map<std::size_t, std::unique_ptr<Context>> g_contexts_;

		std::mutex mt;
	} // anonymous namespace


	handle_t connect(std::size_t bulk) {
		std::lock_guard lock_guard(mt);

		auto it = g_contexts_.find(bulk);

		if (it != g_contexts_.end()) {
			it->second->inc_connection();
			return it->second.get(); // c-style out of this functions, so we can
		} else {
			g_contexts_.emplace(bulk, std::make_unique<Context>(bulk));
			return g_contexts_.find(bulk)->second.get();
		}
	}

	void receive(handle_t handle, const char *data, std::size_t size) {
		std::lock_guard lock_guard(mt);

		Context *context_ptr = static_cast<Context *>(handle);
		context_ptr->input(std::string(data, size));
	}

	void disconnect(handle_t handle) {
		std::lock_guard lock_guard(mt);

		Context *context_ptr = static_cast<Context *>(handle);
		context_ptr->dec_connection();

		if (!context_ptr->connections()) {
			context_ptr->block_queue_input_and_stop_waiters();
			g_contexts_.erase(context_ptr->get_bulk_size());
		}
	}
} // namespace otus::async
