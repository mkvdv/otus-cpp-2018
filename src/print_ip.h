/*!
\file
\brief Header file with all HA's functions

This file contain all print_ip functions, which used in main programm and tests.
*/

#pragma once

#include "my_traits.h"
#include "utils.h"

#include <type_traits>
#include <iostream>
#include <utility>
#include <tuple>
#include <sstream>

namespace otus {
	namespace details {
		//! Print integral argument as ip address to stringstream
		//! \tparam T integral type
		//! \param[in] val integral value, that will be printed
		//! \param[out] ss stream where ip-filter representation will be printed
		template<class T>
		typename std::enable_if<std::is_integral<T>::value, void>::type
		convert_to_ipstr(const T val, std::stringstream &ss) {
			using unsigned_t = std::make_unsigned_t<T>;
			const unsigned_t unsigned_val = static_cast<unsigned_t >(val);
			const size_t nbytes = sizeof(unsigned_t);

			for (size_t i = 0; i != nbytes; ++i) {
				if (i != 0) {
					ss << '.';
				}

				if (otus::is_little()) {
					ss << (unsigned int) *(((unsigned char *) &unsigned_val) + (nbytes - i - 1));
				} else {
					ss << (unsigned int) *(((unsigned char *) &unsigned_val) + i);
				}
			}
		}

		//! Print all elements of container as ip address to stringstream
		//! \tparam Cnt type of container of integral types or container types, that has begin() and end() methods
		//! \param[in] cnt cnt container with objects, that will be printed
		//! \param[out] ss stream where ip-filter representation will be printed
		template<class Cnt>
		typename std::enable_if<
			otus::my_traits::has_begin_end<Cnt>::value
				&& (std::is_integral<typename Cnt::value_type>::value
					|| otus::my_traits::has_begin_end<typename Cnt::value_type>::value),
			void>::type
		convert_to_ipstr(const Cnt &cnt, std::stringstream &ss) {
			for (auto it = cnt.begin(); it != cnt.end(); ++it) {
				if (it != cnt.begin()) {
					ss << '.';
				}
				details::convert_to_ipstr(*it, ss);
			}
		}

		//! Print argument string to system output
		//! \param s string, that will be printed
		//! \param[out] ss stream where ip-filter representation will be printed
		void convert_to_ipstr(const std::string &s, std::stringstream &ss) {
			ss << s;
		}

		namespace variadic {
			template<class T>
			void covert_variadic_to_ipstr(std::stringstream &ss, T &&arg) {
				convert_to_ipstr(std::forward<T>(arg), ss);
			}

			template<class T, class ...Args>
			void covert_variadic_to_ipstr(std::stringstream &ss, T &&arg, Args &&... args) {
				convert_to_ipstr(std::forward<T>(arg), ss);
				ss << '.';
				covert_variadic_to_ipstr(ss, args...);
			}

			//! Wrapper for deducing template index sequence and convert tuple to argument list.
			//! \tparam Args tuple argument types
			//! \tparam Is deduced index sequence
			//! \param tpl tuple with equal types, tha t will be printed as ip addr
			//! \param ss utput stream for printing
			template<class ...Args, size_t ...Is>
			void convert_tpl_to_ipstr(const std::tuple<Args...> &tpl,
			                          std::index_sequence<Is...>,
			                          std::stringstream &ss) {
				covert_variadic_to_ipstr(ss, std::get<Is>(tpl)...);
			}
		} // namespace variadic

		//! Print tuple of equal type arguments using other print_ip functions
		//! \tparam Args
		//! \param tpl
		//! \param[out] ss stream where ip-filter representation will be printed
		template<class ...Args>
		typename std::enable_if<
			sizeof...(Args) && otus::my_traits::equal_types<Args...>::value, void>::type
		convert_to_ipstr(const std::tuple<Args...> &tpl, std::stringstream &ss) {
			variadic::convert_tpl_to_ipstr(tpl, std::index_sequence_for<Args...>{}, ss);
		}

		//! Do nothing with empty tuple
		void convert_to_ipstr(std::tuple<>, std::stringstream &) {}

	} // namespace details

	/// Print argument as ip address.
	///
	/// \tparam T integral type or container or tuple of equal types.
	/// \param[in] val value that will be printed as ip-address
	template<class T>
	void print_ip(T &&val) {
		std::stringstream ss;
		details::convert_to_ipstr(std::forward<T>(val), ss);
		std::cout << ss.str();
	}

} // namespace otus