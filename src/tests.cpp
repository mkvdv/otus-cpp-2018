#include "mylist.h"
#include "allocator.h"

#include <vector>
#include <map>

namespace {
  namespace details {
    int factorial(int x) {
        return x <= 1 ? 1 : x * factorial(x - 1);
    }

    std::vector<int> gen_factorials(size_t n) {
        std::vector<int> res;
        res.reserve(n);
        for (size_t i = 0; i != n; ++i) {
            res.push_back(factorial(static_cast<int>(i)));
        }
        return res;
    }

    template<class Map>
    void test_map(Map &map, const std::vector<int> &factorials10, size_t CACHE_SIZE) {
        for (size_t i = 0; i != CACHE_SIZE; ++i) {
            map[i] = factorials10[i];
        }
        for (std::pair<const size_t, size_t> &p: map) {
            std::cout << p.first << ' ' << p.second << '\n';
        }
        std::cout << '\n';
    }

    template<class List>
    void test_list(List &lst, size_t CACHE_SIZE) {
        for (int i = static_cast<int>(CACHE_SIZE - 1); i >= 0; --i) {
            lst.push_front(i); // push in "reverse" order, cause we push to head
        }
        for (auto &elem: lst) {
            std::cout << elem << ' ';
        }
        std::cout << '\n';
    }
  } // namespace details

  void test() {
      const size_t CACHE_SIZE = 10;
      std::vector<int> factorials10 = details::gen_factorials(10);

      std::map<size_t, size_t> map;
      details::test_map(map, factorials10, CACHE_SIZE);

      std::map<size_t, size_t, std::less<>,
               otus::otus_allocator<std::pair<const int, int>, CACHE_SIZE>> map10;
      details::test_map(map10, factorials10, CACHE_SIZE);

      otus::mylist<int> lst;
      details::test_list(lst, CACHE_SIZE);

      otus::mylist<int, otus::otus_allocator<int, CACHE_SIZE>> lst10;
      details::test_list(lst10, CACHE_SIZE);
  }
} // anonymous namespace

int main() {
    test();

    return 0;
}