#include "filters.h"

#include <cassert>
#include <fstream>

//#define DEBUG

namespace {
	const int ERR_CODE = -1;
} // anonymous namespace

int main()
{
	using namespace otus;

	try {
		std::vector<ip_t> ip_pool;

		for (std::string line; getline(std::cin, line);) {
			assert(line != "\n" && line != "\t" && !line.empty());

			std::vector<std::string> v = split<std::string>(line, '\t', identity());
			ip_pool.push_back(split<std::byte>(v.at(0), '.', [](const std::string &s) -> std::byte {
			  return static_cast<std::byte>((stoul(s)));
			}));
		}

		// reverse lexicographically sort
		sort(ip_pool.begin(),
			 ip_pool.end(),
			 [](const ip_t &a, const ip_t &b) {
			   return std::lexicographical_compare(a.cbegin(), a.cend(),
												   b.cbegin(), b.cend(),
												   std::greater<>());
			 });

#ifdef DEBUG
		std::cout << "######## reverse lexicographically sort" << std::endl;
#endif
		print(ip_pool);

		// filter by first std::byte and output
		auto ips1 = filter(static_cast<std::byte>(1), ip_pool);
#ifdef DEBUG
		std::cout << "######## filter(1)" << std::endl;
#endif
		print(ips1);

		// filter by first and second std::bytes and output
		auto ips2 =
			filter(static_cast<const std::byte>(46), static_cast<const std::byte>(70), ip_pool);
#ifdef DEBUG
		std::cout << "######## filter(46, 70)" << std::endl;
#endif
		print(ips2);

		// filter by any std::byte and output
		auto ips_any = filter_any(static_cast<const std::byte>(46), ip_pool);
#ifdef DEBUG
		std::cout << "######## filter_any(46)" << std::endl;
#endif
		print(ips_any);

	}
	catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		return ERR_CODE;
	}

	return 0;
}
