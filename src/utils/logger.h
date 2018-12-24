/*!
\file
\brief File with simple logger class declaration.
*/

#pragma once

#include <string>

namespace otus::utils {
	class Logger {
	public:
		explicit Logger(const char *location);
		void info(const std::string &mesg) const;
	private:
		const char *location_{}; // where logger was created, passed to ctor as param
	};
} // namespace otus::utils