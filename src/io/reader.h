#pragma once

#include "ireader.h"

#include <istream>

namespace otus {
	/**
	 * Read from istream after started, sen all readed strings to Listeners
	 * inherits from Listenable
	 */
	class Reader: public IReader {
	public:
		explicit Reader(std::istream &istream);
		void start_reading_cycle() override;
	private:
		std::istream &istream_;
	};
} // namespace otus
