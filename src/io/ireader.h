#pragma once

#include "../utils/listener.h"
#include <string>


namespace otus {
	class IReader: public Listenable<std::string> {
	public:
		virtual void start_reading_cycle() = 0;
	};
} // namespace otus
