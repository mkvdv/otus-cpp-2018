/*!
\file
\brief Header file with some trait classes
*/

#pragma once

#include <type_traits>

namespace otus::my_traits {
	//! Trait class, value is true if template type T has begin() and end() methods in compile-time
	//! \tparam T user type for checking
	template<class T>
	struct has_begin_end {
		typedef char yes;
		typedef struct { yes dummy[2]; } no;

		template<typename U>
		static auto test(U *u) -> decltype((*u).begin(), (*u).end(), yes()); // comma operator

		static no test(...);

		enum {
			value = (sizeof(yes) == sizeof test((T *) nullptr))
		};
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////

	//! Trait class for checking, that all passed template argument types are equal.
	//! \tparam T,Args... type parameters, that will be checked for equality.
	template<class T, class ... Args>
	struct equal_types;

	template<class T>
	struct equal_types<T> {
		static constexpr bool value = true;
	};

	template<class A, class B, class ... Args>
	struct equal_types<A, B, Args...> {
		static constexpr bool
			value = std::is_same<A, B>::value && equal_types<B, Args...>::value;
	};
} // namespace otus
