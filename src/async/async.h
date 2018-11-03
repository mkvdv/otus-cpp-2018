#pragma once

#include <cstddef>

namespace otus::async {
	using handle_t = void *;

	/**
	 * Create new execution Context, if it called just first time with this bulk argument
	 * or return pointer to already create global Context object.
	 * @param bulk number of "commands" in every bulk
	 * @return pointer to Context, casted to void* for unification? maybe
	 */
	handle_t connect(std::size_t bulk);

	/**
	 * Write data to inputstream of context_executor.
	 * @param handle pointer to context
	 * @param data, size -- input buffer
	 */
	void receive(handle_t handle, const char *data, std::size_t size);

	/**
	 * Destroy Context, if there is no any connections (Context count connections to its)
	 * @param handle -- pointer to Context
	 */
	void disconnect(handle_t handle);
} // namespace otus::async
