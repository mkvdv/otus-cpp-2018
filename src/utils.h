#pragma once

#include <vector>
#include <iostream>
#include <cstddef>
#include <functional>
#include <sstream>
#include <cstdint>

namespace otus {
	struct identity {
		template<class T>
		constexpr T &&operator()(T &&t) const noexcept
		{
			return std::forward<T>(t);
		}
	};

	using ip_t = std::vector<uint8_t>;

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

	namespace code_review_discussion {
		template<class Less>
		struct lexicographical_ip_comparator {
			/**
			 * @return true if (less(a, b) == true), false otherwise.
			 */
			bool operator()(const ip_t &a, const ip_t &b) const
			{
				Less less;
				size_t beg = 0;
				size_t a_end = a.size();
				size_t b_end = b.size();

				while (beg != a_end && beg != b_end) {
					if (less(a[beg], b[beg])) {
						return true;
					}
					if (less(b[beg], a[beg])) {
						return false;
					}
					++beg;
				}
				return beg == a_end && beg != b_end;
			}
		};
	}

} // namespace otus
