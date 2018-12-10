#include "../src/utils/utils.h"
#include "../src/filter/filters.h"
#include <ostream>

// todo jesus christ, what am I doing, it is f***ing dangerous!!!
// otherwise boost::test fails inside itself
namespace std {
  std::ostream &operator<<(std::ostream &os, const std::vector<uint8_t> &vec) {
      for (uint8_t elem : vec) {
          os << elem << ' ';
      }
      return os;
  }
}

//____________________________________________________________________________//

#define BOOST_TEST_MODULE test_filter

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(test_suite_split)

  BOOST_AUTO_TEST_CASE(test_split) {
      using namespace otus;
      std::string str = "192.168.0.1";
      ip_t result = split<uint8_t>(str, '.', [](const std::string &s) {
          return static_cast<uint8_t>((stoul(s)));
      });
      ip_t expected = {uint8_t(192), uint8_t(168), uint8_t(0), uint8_t(1)};

      BOOST_CHECK_EQUAL(expected, result);
  }

BOOST_AUTO_TEST_SUITE_END()

//____________________________________________________________________________//

namespace {
  struct Fixture {
    Fixture() {
        BOOST_TEST_MESSAGE("setup fixture");
        ip_pool_.reserve(POOL_SIZE);
        for (size_t i = 0; i < POOL_SIZE; ++i) {
            ip_pool_.emplace_back(otus::ip_t{
                (uint8_t) (i % MODULE), (uint8_t) ((i + 5) % MODULE),
                (uint8_t) ((i + 7) % MODULE), (uint8_t) ((i + 11) % MODULE)
            });
        }
    }

    ~Fixture() {
        BOOST_TEST_MESSAGE("teardown fixture");
    }

    std::vector<otus::ip_t> ip_pool_{};
    const size_t POOL_SIZE = 1024;
    const uint8_t b{42};
    const uint8_t MODULE = 255u;
  };
}

//____________________________________________________________________________//

BOOST_FIXTURE_TEST_SUITE(test_suite_filter, Fixture)

  BOOST_AUTO_TEST_CASE(test_case1) {
      std::vector<std::reference_wrapper<const otus::ip_t>> res = otus::filter(b, ip_pool_);
      for (auto &re : res) {
          BOOST_CHECK_EQUAL(re.get()[0], b);
      }
  }

//____________________________________________________________________________//

  BOOST_AUTO_TEST_CASE(test_case2) {
      std::vector<std::reference_wrapper<const otus::ip_t>> res = otus::filter(b, b, ip_pool_);
      for (auto &re : res) {
          BOOST_ASSERT(re.get()[0] == b && re.get()[1] == b);
      }
  }

//____________________________________________________________________________//

  BOOST_AUTO_TEST_CASE(test_case3) {
      std::vector<std::reference_wrapper<const otus::ip_t>> res = otus::filter_any(b, ip_pool_);
      for (auto &re : res) {
          BOOST_ASSERT(re.get()[0] == b ||
              re.get()[1] == b ||
              re.get()[2] == b ||
              re.get()[3] == b);
      }
  }

//____________________________________________________________________________//

BOOST_AUTO_TEST_SUITE_END()