#pragma once

#include <string>

namespace otus {
	class ICommand {
	public:
		virtual void exec() = 0;
		virtual std::string get_name() const = 0;
		virtual ~ICommand() = default;
	};
} // namespace otus