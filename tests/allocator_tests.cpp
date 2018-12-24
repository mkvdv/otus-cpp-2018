#include <gtest/gtest.h>

#include "../src/allocator.h"
#include "test_struct.h"

namespace {
  const size_t CAPACITY = 10;
}

/**
 * valgrind and ASAN should no produce any errors
 */
TEST(GtestSuiteAllocator, ctor_move) {
    using alloc_t = otus::otus_allocator<S, CAPACITY>;
    alloc_t allocator;
    S *p = allocator.allocate(1);
    ASSERT_TRUE(p);

    alloc_t allocator2 = std::move(allocator);
    allocator2.deallocate(p, 1);
    // valgrind and ASAN should no produce any errors
}

TEST(GtestSuiteAllocator, allocate_construct_destroy_deallocate) {
    using T = std::vector<std::string>;
    using alloc_t = otus::otus_allocator<T, CAPACITY>;
    alloc_t allocator;

    T *p1 = allocator.allocate(1);
    ASSERT_TRUE(p1);

    allocator.construct(p1, std::vector{std::string("abc")});
    ASSERT_EQ(p1->at(0), "abc");

    allocator.destroy(p1);
    allocator.deallocate(p1, 1);

    // valgrind and ASAN should no produce any errors
}

TEST(GtestSuiteAllocator, allocate1) {
    using T = std::vector<std::string>;
    using alloc_t = otus::otus_allocator<T, CAPACITY>;
    alloc_t allocator;

    T *p1 = allocator.allocate(1);
    ASSERT_TRUE(p1);

    allocator.construct(p1, std::vector{std::string("abc")});
    ASSERT_EQ(p1->at(0), "abc");

    allocator.destroy(p1);
    allocator.deallocate(p1, 1);

    // valgrind and ASAN should no produce any errors
}

TEST(GtestSuiteAllocator, allocate2) {
    using T = std::vector<std::string>;
    using alloc_t = otus::otus_allocator<T, CAPACITY>;
    alloc_t allocator;

    T *p1 = allocator.allocate(2);
    T *p2 = p1 + 1;
    ASSERT_TRUE(p1);

    allocator.construct(p1, std::vector{std::string("abc")});
    allocator.construct(p2, std::vector{std::string("cde")});
    ASSERT_EQ(p1->at(0), "abc");
    ASSERT_EQ(p2->at(0), "cde");

    allocator.destroy(p1);
    allocator.destroy(p2);
    allocator.deallocate(p1, 2);

    // valgrind and ASAN should no produce any errors
}

TEST(GtestSuiteAllocator, vector_usage) {
    using T = std::vector<std::string>;
    using Cnt = std::vector<T, otus::otus_allocator<T, CAPACITY>>;

    Cnt vec;
    // create
    for (size_t i = 0; i != 10 * CAPACITY; ++i) {
        vec.push_back(std::vector{
            std::to_string(i),
            std::to_string(i * 10),
            std::to_string(i * 100)});
    }

    // modify
    const std::string ADDED_STR = "ADDED";
    for (T &vec_of_str: vec) {
        vec_of_str.pop_back();
        std::swap(vec_of_str[0], vec_of_str[1]);
        vec_of_str.push_back(ADDED_STR);
    }

    // check
    for (size_t i = 0; i != vec.size(); ++i) {
        T &vec_of_str = vec[i];
        ASSERT_EQ(vec_of_str[0], std::to_string(i * 10));
        ASSERT_EQ(vec_of_str[1], std::to_string(i));
        ASSERT_EQ(vec_of_str[2], ADDED_STR);
    }
}
