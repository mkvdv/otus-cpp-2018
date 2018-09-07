/*!
\file
\brief File with implementation of util functions

This file contain implementation of all utils functions, declared in header file
 utils.h
*/

#include "utils.h"

#include <cstdint>

namespace otus {
	//! Check that byte order is big-endian
	//! \todo make it constexpr
	//! \return true if byte order is big-endian
	bool is_big() {
		union {
			uint32_t i;
			uint8_t c[4];
		} check = {
			0x01'00'00'00 // ISO C++ does not allow C99 designated initializers
		};

		return check.c[0];
	}

	//! Check that byte order is little-endian
	//! \return true if byte order is little-endian
	bool is_little() {
		return !is_big();
	}
} // namespace otus
