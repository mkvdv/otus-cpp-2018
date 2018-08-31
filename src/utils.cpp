#include "utils.h"

namespace otus {
	namespace details {
		void print_ip(const ip_t &ip)
		{
			using namespace std;
			for (auto ip_part = ip.cbegin(); ip_part != ip.cend(); ++ip_part) {
				if (ip_part != ip.cbegin()) {
					cout << '.';
				}
				cout << static_cast<unsigned >(*ip_part);
			}
		}
	}

	void print(const std::vector<ip_t> &ip_pool)
	{
		for (const ip_t &ip : ip_pool) {
			details::print_ip(ip);
			std::cout << std::endl;
		}
	}

	void print(const std::vector<std::reference_wrapper<const ip_t>> &ip_pool)
	{
		for (std::reference_wrapper<const ip_t> ip : ip_pool) {
			details::print_ip(ip.get());
			std::cout << std::endl;
		}
	}

} // namespace otus