#include "../src/mylist.h"
#include "../src/allocator.h"

#include <gtest/gtest.h>

#include "test_struct.h"

TEST(GtestSuiteMylist, construct_copy_move) {
    using List = otus::mylist<S, otus::otus_allocator<S, 10>>;
    List lst10;
    const size_t ELEMENTS = 12;

    for (int j = 0; j != ELEMENTS; ++j) {
        lst10.push_front(S(j, 10));
    }

    ASSERT_EQ(ELEMENTS, lst10.size());

    List copy = lst10;
    ASSERT_EQ(ELEMENTS, copy.size());

    {
        auto it = lst10.begin();
        auto it_copy = copy.begin();
        for (size_t i = 1; i != ELEMENTS + 1; ++i) {
            ASSERT_EQ((*it).get(), (*it_copy).get());
            ASSERT_NE(&(*it), &(*it_copy));
            ++it;
            ++it_copy;
        }
    }

    List moved = std::move(lst10);
    {
        auto it = moved.begin();
        auto it_copy = copy.begin();
        for (size_t i = 1; i != ELEMENTS + 1; ++i) {
            ASSERT_EQ((*it).get(), (*it_copy).get());
            ASSERT_NE(&(*it), &(*it_copy));
            ++it;
            ++it_copy;
        }
    }
}

TEST(GtestSuiteMylist, push_front) {

    otus::mylist<S, otus::otus_allocator<S, 10>> lst10;
    const size_t ELEMENTS = 12;

    for (int j = ELEMENTS; j != 0; --j) {
        lst10.push_front(S(j, 10));
    }

    auto it = lst10.begin();
    for (size_t i = 1; i != ELEMENTS + 1; ++i) {
        ASSERT_EQ(i * 10, (*it).get());
        ++it;
    }
}

TEST(GtestSuiteMylist, emplace_front) {
    class S {
     public:
        S(int a, char b)
            : value_(static_cast<const unsigned long long int>(a * b)) {}

        unsigned long long get() const {
            return value_;
        }

     private:
        const unsigned long long value_;
    };

    otus::mylist<S, otus::otus_allocator<S, 10>> lst10;
    const size_t ELEMENTS = 12;

    for (size_t j = ELEMENTS; j != 0; --j) {
        lst10.emplace_front(j, 10);
    }

    auto it = lst10.begin();
    for (size_t i = 1; i != ELEMENTS + 1; ++i) {
        ASSERT_EQ(i * 10, (*it).get());
        ++it;
    }
}

