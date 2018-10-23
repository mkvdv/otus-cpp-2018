#pragma once

#include <boost/concept_check.hpp>

namespace otus {
	template<class It>
	struct RandomAccessIterator: boost::BidirectionalIterator<It>, boost::Comparable<It> {
	private:
		typedef std::iterator_traits<It> t;

	public:
		using value_type = typename t::value_type;
		using difference_type = typename t::difference_type;
		using reference = typename t::reference;
		using pointer = typename t::pointer;
		using iterator_category = typename t::iterator_category;

	private:
		It i;
		const It a;
		const It b;
		difference_type n;
		value_type v;

	public:
		BOOST_CONCEPT_ASSERT((boost::Convertible<iterator_category, std::random_access_iterator_tag>));

		BOOST_CONCEPT_USAGE(RandomAccessIterator) {
			It &r = i;

			It &ref_to_it1 = (r += n);

			It it1 = a + n; // required operator+(const It&, const ptrdiff&)
			It it2 = n + a; // required operator+(const ptrdiff&, const It&)

			It &ref_to_it2 = (r -= n);

			It it3 = i - n;

			difference_type dif = b - a;
			reference ref_to_T = i[n];
		}
	}; // struct RandomAccessIterator<T>
} // namespace otus
