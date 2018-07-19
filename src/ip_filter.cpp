#include "utils.h"

#include <cassert>
#include <fstream>
#include <sstream>

#define DEBUG

const int ERR_CODE = -1;

/**
 * ("",  '.') -> [""]
 * ("11", '.') -> ["11"]
 * ("..", '.') -> ["", "", ""]
 * ("11.", '.') -> ["11", ""]
 * (".11", '.') -> ["", "11"]
 * ("11.22", '.') -> ["11", "22"]
 */
template<class T, class Preprocessor>
std::vector<T> split(const std::string &str, char delimiter, Preprocessor p)
{
	using namespace std;
	vector<T> res;
	res.reserve(4);

	string::size_type start = 0;
	string::size_type stop = str.find_first_of(delimiter);
	while (stop != string::npos) {
		res.push_back(p(str.substr(start, stop - start)));

		start = stop + 1;
		stop = str.find_first_of(delimiter, start);
	}

	res.push_back(p(str.substr(start)));

	return res;
}

template<class Less>
struct lexicographical_ip_comparator {
	/**
	 * @return true if lhs < rhs, false otherwise.
	 */
	bool operator()(const ip_t &lhs, const ip_t &rhs) const
	{
		if (lhs.size() != 4 || rhs.size() != 4) {
			std::stringstream ss;
			ss << "Incorect vector size: some of " << lhs.size() << " or " << rhs.size()
			   << " is not equal 4.";
			throw std::runtime_error(ss.str());
		}
		Less less;
		return less(lhs[0], rhs[0]) || (lhs[0] == rhs[0]
			&& (less(lhs[1], rhs[1]) || (lhs[1] == rhs[1]
				&& (less(lhs[2], rhs[2]) || (lhs[2] == rhs[2]
					&& less(lhs[3], rhs[3]))))));
	}
};

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

int main()
{
	using namespace std;

	try {
		vector<ip_t> ip_pool;

		for (string line; getline(cin, line);) {
			assert(line != "\n" && line != "\t" && !line.empty());

			vector<string> v = split<string>(line, '\t', identity());
			ip_pool.push_back(split<byte>(v.at(0), '.', [](const string &s) -> byte {
			  return static_cast<byte>((stoul(s)));
			}));
		}

		// reverse lexicographically sort
		sort(ip_pool.begin(),
			 ip_pool.end(),
			 lexicographical_ip_comparator<std::greater<>>());
#ifdef DEBUG
		cout << "######## reverse lexicographically sort" << endl;
#endif
		print(ip_pool);

		// filter by first byte and output
		auto ips1 = filter(static_cast<byte>(1), ip_pool);
#ifdef DEBUG
		cout << "######## filter(1)" << endl;
#endif
		print(ips1);

		// filter by first and second bytes and output
		auto ips2 = filter(static_cast<const byte>(46), static_cast<const byte>(70), ip_pool);
#ifdef DEBUG
		cout << "######## filter(46, 70)" << endl;
#endif
		print(ips2);

		// filter by any byte and output
		auto ips_any = filter_any(static_cast<const byte>(46), ip_pool);
#ifdef DEBUG
		cout << "######## filter_any(46)" << endl;
#endif
		print(ips_any);

	}
	catch (const exception &e) {
		cerr << e.what() << endl;
		return ERR_CODE;
	}

	return 0;
}
