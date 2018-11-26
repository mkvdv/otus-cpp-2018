#include "../src/model/simple_table.h"

#define BOOST_TEST_MODULE test_main

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(test_suite_main)

  BOOST_AUTO_TEST_CASE(test_truncate) {
	  using namespace otus::table;

	  otus::table::SimpleTable t1("A", 1);
	  t1.insert(1, otus::table::simple_row_t{std::string{"1"}});
	  t1.insert(2, otus::table::simple_row_t{std::string{"2"}});
	  t1.insert(4, otus::table::simple_row_t{std::string{"4"}});
	  BOOST_ASSERT(t1.size() == 3);

	  otus::table::SimpleTable t2("B", 1);
	  t2.insert(1, otus::table::simple_row_t{std::string{"aa"}});
	  t2.insert(3, otus::table::simple_row_t{std::string{"bb"}});
	  BOOST_ASSERT(t2.size() == 2);

	  t1.truncate();
	  BOOST_ASSERT(t1.empty());

	  t2.truncate();
	  BOOST_ASSERT(t2.empty());
  }

  BOOST_AUTO_TEST_CASE(test_operations) {
	  otus::table::SimpleTable t1("A2", 1);
	  otus::table::SimpleTable t2("B2", 1);

	  BOOST_ASSERT(otus::table::intersection(t1, t2).empty());
	  BOOST_ASSERT(otus::table::symmetric_difference(t1, t2).empty());

	  t1.insert(2, otus::table::simple_row_t{std::string{"2"}});
	  t1.insert(4, otus::table::simple_row_t{std::string{"4"}});
	  t2.insert(3, otus::table::simple_row_t{std::string{"bb"}});

	  BOOST_ASSERT(otus::table::intersection(t1, t2).empty());
	  BOOST_ASSERT(otus::table::symmetric_difference(t1, t2).size() == 3);

	  t1.insert(1, otus::table::simple_row_t{std::string{"1"}});
	  t2.insert(1, otus::table::simple_row_t{std::string{"aa"}});

	  BOOST_ASSERT(otus::table::intersection(t1, t2).size() == 1);
	  BOOST_ASSERT(otus::table::symmetric_difference(t1, t2).size() == 3);
  }

BOOST_AUTO_TEST_SUITE_END()
