#pragma once

#include "utils.h"
#include <vector>

std::vector<std::reference_wrapper<const ip_t>>
filter(std::byte first_byte, const std::vector<ip_t> &ip_pool);

std::vector<std::reference_wrapper<const ip_t>>
filter(std::byte first_byte,
	   std::byte second_byte,
	   const std::vector<ip_t> &ip_pool);

std::vector<std::reference_wrapper<const ip_t>>
filter_any(std::byte any_byte, const std::vector<ip_t> &ip_pool);
