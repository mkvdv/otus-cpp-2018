#pragma once

#include <cstddef>

namespace otus::async {
	using handle_t = void *;

	/**
	 * Create new execution Context
	 * @param bulk number of "commands" in every bulk
	 * @return unique number for every connected context
	 */
	handle_t connect(std::size_t bulk);

	/**
	 * Write data to inputstream of context_executor.
	 * @param data, size -- input buffer
	 */
	void receive(handle_t handle, const char *data, std::size_t size);

	/**
	 * Destroy Context, if there is no any connections (Context count connections to its)
	 */
	void disconnect(handle_t handle);
} // namespace otus::async
