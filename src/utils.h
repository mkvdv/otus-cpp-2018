/*!
\file
\brief Header file util functions

This file contain all print_ip functions, which used in main program and tests.
 Functions defined in header, because constexpr function MUST be defined before used.
*/

#pragma once

#include "utils.h"
#include <cstdint>

namespace otus {
	namespace details {
		class Endian {
		private:
			static constexpr uint32_t uint32_ = 0x01020304;
			static constexpr uint8_t magic_ = (const uint8_t &) uint32_;
		public:
			static constexpr bool little = magic_ == 0x04;
			static constexpr bool middle = magic_ == 0x02;
			static constexpr bool big = magic_ == 0x01;
			static_assert(little || middle || big, "Cannot determine endianness!");

			Endian() = delete;
		};
	} // namespace details

//! Check that byte order is big-endian
//! \todo make it constexpr
//! \return true if byte order is big-endian
	constexpr bool is_big() {
		return details::Endian::big;
	}

//! Check that byte order is little-endian
//! \return true if byte order is little-endian
	constexpr bool is_little() {
		return details::Endian::little;
	}
} // namespace otus

