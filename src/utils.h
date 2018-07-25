#pragma once

#include <vector>
#include <iostream>
#include <cstddef>
#include <functional>
#include <sstream>

namespace otus {
	struct identity {
		template<class T>
		constexpr T &&operator()(T &&t) const noexcept
		{
			return std::forward<T>(t);
		}
	};

	using ip_t = std::vector<std::byte>;

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
				size_t a_beg = 0;
				size_t b_beg = 0;
				size_t a_end = a.size();
				size_t b_end = b.size();

				while (a_beg != a_end && b_beg != b_end) {
					if (less(a[a_beg], b[b_beg])) {
						return true;
					}
					if (less(b[b_beg], a[a_beg])) {
						return false;
					}
					++a_beg;
					++b_beg;
				}
				return a_beg == a_end && b_beg != b_end;
			}
		};
	}

} // namespace otus
