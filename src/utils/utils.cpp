#include "utils.h"
#include <iostream>

namespace otus {
  namespace details {
    void print_ip(const ip_t &ip, std::ostream &os) {
        using namespace std;
        for (auto ip_part = ip.cbegin(); ip_part != ip.cend(); ++ip_part) {
            if (ip_part != ip.cbegin()) {
                os << '.';
            }
            os << (uint32_t) *ip_part;
        }
    }
  }

  void print(const std::vector<ip_t> &ip_pool, std::ostream &os) {
      for (const ip_t &ip : ip_pool) {
          details::print_ip(ip, os);
          os << std::endl;
      }
  }

  void print(const std::vector<std::reference_wrapper<const ip_t>> &ip_pool, std::ostream &os) {
      for (std::reference_wrapper<const ip_t> ip : ip_pool) {
          details::print_ip(ip.get(), os);
          os << '\n';
      }
  }
} // namespace otus
