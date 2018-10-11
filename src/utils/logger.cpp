/*!
\file
\brief File with implementation of logger.
*/

#include "logger.h"

#include <iostream>
#include <iomanip>
#include <cassert>

namespace otus::utils {
	Logger::Logger(const char* location)
			:location_(location) {
		assert(location_);
	}

	void Logger::info(const std::string& mesg) const
	{
		std::cout << "[Info from " << std::setw(17) << location_ << "] : " << mesg << '\n';
	}
} // namespace otus::utils
