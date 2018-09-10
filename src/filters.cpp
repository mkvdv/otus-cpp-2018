#include "filters.h"

namespace otus {
	std::vector<std::reference_wrapper<const ip_t>>
	filter(uint8_t first_byte, const std::vector<ip_t> &ip_pool)
	{
		using namespace std;
		std::vector<std::reference_wrapper<const ip_t>> res;

		for (const auto &ip: ip_pool) {
			if (ip[0] == first_byte) {
				res.push_back(std::cref(ip));
			}
		}

		return res;
	}

	std::vector<std::reference_wrapper<const ip_t>>
	filter(const uint8_t first_byte,
		   const uint8_t second_byte,
		   const std::vector<ip_t> &ip_pool)
	{
		using namespace std;
		std::vector<std::reference_wrapper<const ip_t>> res;

		for (const auto &ip: ip_pool) {
			if (ip[0] == first_byte && ip[1] == second_byte) {
				res.push_back(std::cref(ip));
			}
		}

		return res;
	}

	std::vector<std::reference_wrapper<const ip_t>>
	filter_any(const uint8_t any_byte, const std::vector<ip_t> &ip_pool)
	{
		using namespace std;
		vector<reference_wrapper<const ip_t>> res;

		for (const auto &ip: ip_pool) {
			if (ip[0] == any_byte || ip[1] == any_byte || ip[2] == any_byte || ip[3] == any_byte) {
				res.push_back(std::cref(ip));
			}
		}

		return res;
	}
} // namespace otus