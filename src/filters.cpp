#include "filters.h"

std::vector<std::reference_wrapper<const ip_t>>
filter(std::byte first_byte, const std::vector<ip_t> &ip_pool)
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
filter(const std::byte first_byte,
	   const std::byte second_byte,
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
filter_any(const std::byte any_byte, const std::vector<ip_t> &ip_pool)
{
	using namespace std;
	std::vector<std::reference_wrapper<const ip_t>> res;

	for (const auto &ip: ip_pool) {
		if (ip[0] == any_byte || ip[1] == any_byte || ip[2] == any_byte || ip[3] == any_byte) {
			res.push_back(std::cref(ip));
		}
	}

	return res;
}