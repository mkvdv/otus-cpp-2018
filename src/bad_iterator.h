#pragma once

#include <cstddef>
#include <iterator>

namespace otus {
	template<class T>
	class BadIterator {
	public:
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using reference = T &;
		using pointer =T *;
		using iterator_category = std::random_access_iterator_tag;

	public:
		BadIterator &operator++() { return *this; };
		reference operator*() const { return test; };

	private:
		T test;
	};

} // namespace otus
