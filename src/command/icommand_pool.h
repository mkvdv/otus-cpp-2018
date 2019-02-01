#pragma once

#include "icommand.h"

#include <memory>
#include <chrono>
#include <vector>

namespace otus {
	class ICommandPool {
	public:
		virtual void add_command(std::unique_ptr<ICommand> &&new_cmd) = 0;
		virtual size_t size() const = 0;
      virtual std::vector<std::string> run_all_commands_and_clear() = 0;
		virtual std::chrono::time_point<std::chrono::system_clock> get_first_cmd_time_point() const = 0;

		virtual ~ICommandPool() = default;
	};
} // namespace otus
