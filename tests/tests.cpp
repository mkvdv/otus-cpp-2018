#include "../src/radix_tree/radix_tree.h"

#include <sstream>

#define BOOST_TEST_MODULE test_main

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(test_suite_main)

  BOOST_AUTO_TEST_CASE(test_simple) {
      BOOST_CHECK(true);
      std::stringstream input;
      input << "alek\n"
               "sasha\n"
               "alesha\n"
               "aleks\n"
               "maksim\n"
               "aleksey\n"
               "kisa\n"
               "alena\n"
               "masha\n"
               "rita\n"
               "kiska\n"
               "kriska\n"
               "krual\n"
               "krusha\n"
               "kisusha\n"
               "kikiska\n";

      std::string abbrevation_expected = "alek alek\n"
                                         "aleks aleks\n"
                                         "aleksey alekse\n"
                                         "alena alen\n"
                                         "alesha ales\n"
                                         "kikiska kik\n"
                                         "kisa kisa\n"
                                         "kiska kisk\n"
                                         "kisusha kisu\n"
                                         "kriska kri\n"
                                         "krual krua\n"
                                         "krusha krus\n"
                                         "maksim mak\n"
                                         "masha mas\n"
                                         "rita r\n"
                                         "sasha s\n";

      std::string tree_expected = "├─ale\n"
                                  "|   ├─k$\n"
                                  "|   | ├─s$\n"
                                  "|   | | ├─ey$\n"
                                  "|   ├─na$\n"
                                  "|   ├─sha$\n"
                                  "├─k\n"
                                  "| ├─i\n"
                                  "| | ├─kiska$\n"
                                  "| | ├─s\n"
                                  "| | | ├─a$\n"
                                  "| | | ├─ka$\n"
                                  "| | | ├─usha$\n"
                                  "| ├─r\n"
                                  "| | ├─iska$\n"
                                  "| | ├─u\n"
                                  "| | | ├─al$\n"
                                  "| | | ├─sha$\n"
                                  "├─ma\n"
                                  "|  ├─ksim$\n"
                                  "|  ├─sha$\n"
                                  "├─rita$\n"
                                  "├─sasha$\n";

      otus::RadixTree tree;
      otus::read_from_input(tree, input);

      std::stringstream output_tree;
      tree.print(output_tree);
      BOOST_CHECK_EQUAL(tree_expected, output_tree.str());

      std::stringstream output_abbrevation;
      tree.print_abbreviations(output_abbrevation);
      BOOST_CHECK_EQUAL(abbrevation_expected, output_abbrevation.str());
  }

BOOST_AUTO_TEST_SUITE_END()
