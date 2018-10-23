#pragma once

#include "random_access_iterator_concept.h"

#include <boost/concept_check.hpp>
#include <algorithm>

namespace otus {
	template<typename It>
	void customSort(It beg_it, It end_it) {
		BOOST_CONCEPT_ASSERT((RandomAccessIterator<It>));
		std::sort(beg_it, end_it);
	}

} // namespace otus
