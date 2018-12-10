#include "filters.h"

#include <range/v3/all.hpp>

namespace otus {
  using refs_to_ip = std::vector<std::reference_wrapper<const ip_t>>;

  namespace details {
    template<class Pred>
    refs_to_ip filter_and_push(const std::vector<ip_t> &source, Pred pred) {
        std::vector<std::reference_wrapper<const ip_t>> dest;

        ranges::for_each(source | ranges::view::filter(pred),
                         [&dest](const ip_t &ip) { dest.push_back(std::cref(ip)); });
        return dest;
    }
  }

  refs_to_ip filter(uint8_t first_byte, const std::vector<ip_t> &ip_pool) {
      auto pred = [first_byte](const ip_t &ip) { return ip[0] == first_byte; };
      return details::filter_and_push(ip_pool, pred);
  }

  refs_to_ip filter(const uint8_t first_byte, const uint8_t second_byte,
                    const std::vector<ip_t> &ip_pool) {
      auto pred = [first_byte, second_byte](const ip_t &ip) {
          return ip[0] == first_byte && ip[1] == second_byte;
      };
      return details::filter_and_push(ip_pool, pred);
  }

  refs_to_ip filter_any(const uint8_t any_byte, const std::vector<ip_t> &ip_pool) {
      auto pred = [any_byte](const ip_t &ip) {
          return std::find(ip.begin(), ip.end(), any_byte) != ip.end();
      };
      return details::filter_and_push(ip_pool, pred);
  }
} // namespace otus
