#pragma once

#include <vector>
#include <iostream>
#include <cstddef>
#include <functional>
#include <sstream>

struct identity {
	template<class T>
	constexpr T &&operator()(T &&t) const noexcept
	{
		return std::forward<T>(t);
	}
};

using ip_t = std::vector<std::byte>;

void print_ip(const ip_t &ip);
void print(const std::vector<ip_t> &ip_pool);
void print(const std::vector<std::reference_wrapper<const ip_t>> &ip_pool);

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
			ss << std::string("Incorect vector size: some of ") << lhs.size() << std::string(" or ")
			   << rhs.size()
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
