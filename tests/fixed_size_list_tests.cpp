#include "../src/fixed_size_list.h"

#include <gtest/gtest.h>
#include <random>

#include "test_struct.h"


TEST(GtestSuiteFixedsizelist, construct_move) {
    const size_t CAPACITY = 10;
    using list_t = otus::fixed_size_list<S, CAPACITY>;
    list_t lst;
    std::vector<S *> allocated_pointers;
    for (size_t i = 0; i != CAPACITY; ++i) {
        allocated_pointers.push_back(lst.alloc());
    }
    ASSERT_EQ(CAPACITY, lst.size());

    list_t moved = std::move(lst);
    for (S *p: allocated_pointers) {
        moved.dealloc(p);
    }
}

namespace {
  /**
   * Try to allocate pointer from list and save to vector
   * @return nullptr if can't allocate, else pointer
   */
  template<size_t CAPACITY>
  S *try_alloc(otus::fixed_size_list<S, CAPACITY> &lst,
               std::vector<S *> &allocated_pointers) {
      if (lst.size() == CAPACITY) {
          return nullptr;
      }
      S *p = lst.alloc();
      allocated_pointers.push_back(p);
      return p;
  }

  /**
   * Try to deallocate pointer, extracted from back of vector.
   * @tparam CAPACITY
   * @param lst
   * @param allocated_pointers
   */
  template<size_t CAPACITY>
  void try_dealloc(otus::fixed_size_list<S, CAPACITY> &lst,
                   std::vector<S *> &allocated_pointers) {
      if (lst.size() == 0) {
          return;
      }
      S *p = allocated_pointers.back();
      lst.dealloc(p);
      allocated_pointers.pop_back();
  }
}

TEST(GtestSuiteFixedsizelist, alloc_dealloc) {
    const size_t CAPACITY = 10;
    using list_t = otus::fixed_size_list<S, CAPACITY>;

    list_t lst;
    std::vector<S *> allocated_pointers;

    // allocate construct
    {
        for (int i = 0; i != CAPACITY; ++i) {
            S *p = try_alloc(lst, allocated_pointers);
            new(p) S(i, 10);
        }
        ASSERT_EQ(CAPACITY, lst.size());

        int i = 0;
        for (S *p : allocated_pointers) {
            ASSERT_EQ(10 * i, p->get());
            ++i;
        }
        for (size_t j = 0; j != allocated_pointers.size() - 1; ++j) {
            size_t diff = (char *) allocated_pointers[j + 1] - (char *) allocated_pointers[j];
            ASSERT_EQ(sizeof(list_t::Node), diff);
        }
    }

    // return back to fixed size list
    while (!allocated_pointers.empty()) {
        lst.dealloc(allocated_pointers.back());
        allocated_pointers.pop_back();
    }
    ASSERT_EQ(lst.size(), 0);

    // random allocate and deallocate from list
    {
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<int> dist(0, 1);

        for (size_t i = 0; i != 1000; ++i) {
            if (dist(mt)) {
                try_alloc(lst, allocated_pointers);
            } else {
                try_dealloc(lst, allocated_pointers);
            }
        }

        for (size_t j = allocated_pointers.size(); j != CAPACITY; ++j) {
            try_alloc(lst, allocated_pointers);
        }
        ASSERT_EQ(lst.size(), CAPACITY);
    }

    // check that all allocated pointers are valid and rest of list

    std::set<S *> set(allocated_pointers.begin(), allocated_pointers.end());
    ASSERT_EQ(set.size(), allocated_pointers.size()); // check that all unique

    {
        auto it1 = set.begin();
        auto it2 = ++(set.begin());
        for (size_t i = 0; i != CAPACITY - 1; ++i) {
            size_t diff = (char *) (*it2) - (char *) (*it1);
            ASSERT_EQ(sizeof(list_t::Node), diff);
            ++it1;
            ++it2;
        }
    }

    // deallocate all pointers back to list
    while (!allocated_pointers.empty()) {
        lst.dealloc(allocated_pointers.back());
        allocated_pointers.pop_back();
    }
    ASSERT_EQ(lst.size(), 0);

}

