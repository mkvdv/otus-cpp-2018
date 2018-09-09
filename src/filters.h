#pragma once

#include "utils.h"
#include <vector>

namespace otus {
	std::vector<std::reference_wrapper<const ip_t>>
	filter(uint8_t first_byte, const std::vector<ip_t> &ip_pool);

	std::vector<std::reference_wrapper<const ip_t>>
	filter(uint8_t first_byte,
		   uint8_t second_byte,
		   const std::vector<ip_t> &ip_pool);

	std::vector<std::reference_wrapper<const ip_t>>
	filter_any(uint8_t any_byte, const std::vector<ip_t> &ip_pool);
}