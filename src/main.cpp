#include "filter/filters.h"

#include <cassert>
#include <fstream>
#include <algorithm>
#include <iostream>

#include <range/v3/all.hpp>

namespace {
  void run_ip_filter() {
      using namespace otus;

      std::vector<ip_t> ip_pool;
      std::vector<std::string> input;

      for (std::string line; getline(std::cin, line);) {
          if (line == "\n" || line == "\t" || line.empty()) {
              throw std::runtime_error("Empty strings in input");
          }

          auto lines = ranges::view::split(line, '\t');
          ip_t new_ip;

          auto converter = [](const std::string &s) -> uint8_t {
              return static_cast<uint8_t>((stoul(s)));
          };
          ranges::for_each(
              *std::cbegin(lines) | ranges::view::split('.') | ranges::view::transform(converter),
              [&new_ip](uint8_t v) { new_ip.push_back(v); }
          );

          ip_pool.push_back(std::move(new_ip));
      }

      // reverse lexicographically sort
      ip_pool = std::move(ip_pool) | ranges::action::sort(std::greater<>{});
      print(ip_pool, std::cout);

      // filter_and_push by first uint8_t and output
      auto ips1 = filter(static_cast<uint8_t>(1), ip_pool);
      print(ips1, std::cout);

      // filter_and_push by first and second uint8_ts and output
      auto ips2 = filter(static_cast<const uint8_t>(46), static_cast<const uint8_t>(70), ip_pool);
      print(ips2, std::cout);

      // filter_and_push by any uint8_t and output
      auto ips_any = filter_any(static_cast<const uint8_t>(46), ip_pool);
      print(ips_any, std::cout);
  }
} // anonymous namespace

int main() {
    try {
        run_ip_filter();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
