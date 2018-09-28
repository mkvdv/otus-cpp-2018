/*!
\file
\brief File with implementation of logger.
*/

#include "logger.h"

#include <iostream>
#include <iomanip>

namespace otus::utils {
	Logger::Logger(const char* location)
			:location_(location) { }

	void Logger::info(const char* mesg) const
	{
		std::cout << "[Info from " << std::setw(17) << location_ << "] : " << mesg << '\n';
	}
} // namespace otus::utils
