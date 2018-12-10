#pragma once

#include <vector>
#include <string>
#include <functional>

namespace otus {
  struct identity {
    template<class T>
    constexpr T &&operator()(T &&t) const noexcept {
        return std::forward<T>(t);
    }
  };

  using ip_t = std::vector<uint8_t>;

  void print(const std::vector<ip_t> &ip_pool, std::ostream &os);
  void print(const std::vector<std::reference_wrapper<const ip_t>> &ip_pool, std::ostream &os);

  /**
   * ("",  '.') -> [""]
   * ("11", '.') -> ["11"]
   * ("..", '.') -> ["", "", ""]
   * ("11.", '.') -> ["11", ""]
   * (".11", '.') -> ["", "11"]
   * ("11.22", '.') -> ["11", "22"]
   */
  template<class T, class Preprocessor>
  std::vector<T> split(const std::string &str, char delimiter, Preprocessor p) {
      using namespace std;
      vector<T> res;

      string::size_type start = 0;
      string::size_type stop = str.find_first_of(delimiter);
      while (stop != string::npos) {
          res.emplace_back(p(str.substr(start, stop - start)));

          start = stop + 1;
          stop = str.find_first_of(delimiter, start);
      }

      res.emplace_back(p(str.substr(start)));

      return res;
  }
} // namespace otus
