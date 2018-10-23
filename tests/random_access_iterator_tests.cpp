#include <gtest/gtest.h>

#include <list>
#include <vector>
#include "../src/no_default_constructible.h"
#include "../src/concept_sorting.h"
#include "../src/bad_iterator.h"


TEST(TestRandomAccessIterator, basic) {
//	compile OK
	std::vector<otus::NoDefaultConstructible> values = {{otus::NoDefaultConstructible(5)}};
	otus::customSort(values.begin(), values.end());

//	compile error here
//	std::list<otus::NoDefaultConstructible> lst = {{otus::NoDefaultConstructible(5)}};
//	otus::customSort(lst.begin(), lst.end());

// compile error, there a lot of missed functions and so on
//	otus::customSort(otus::BadIterator<int>{}, otus::BadIterator<int>{});
}
