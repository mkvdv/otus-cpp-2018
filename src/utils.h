#pragma once

#include <cstddef>

namespace otus::utils {
	template<size_t ... Is>
	struct seq {};

	template<size_t N, size_t ... Is>
	struct gen_seq: gen_seq<N - 1, N - 1, Is...> {};

	template<size_t ... Is>
	struct gen_seq<0, Is...>: seq<Is...> {};

} // namespace otus::utils

