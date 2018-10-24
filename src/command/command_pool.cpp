#include "command_pool.h"

#include <sstream>

namespace otus {
	void CommandPool::add_command(std::unique_ptr<ICommand> &&new_cmd) {
		if (commands_to_exec_.empty()) {
			first_command_time_ = std::chrono::system_clock::now();
		}
		commands_to_exec_.push_back(std::move(new_cmd));
	}

	size_t CommandPool::size() const {
		return commands_to_exec_.size();
	}

	void CommandPool::clear() {
		commands_to_exec_.clear();
	}

	std::string CommandPool::run_all_commands_and_clear() {
		std::stringstream ss;

		for (auto it = commands_to_exec_.begin(); it != commands_to_exec_.end(); ++it) {
			(*it)->exec();
			if (it != commands_to_exec_.begin()) {
				ss << ", ";
			}
			ss << (*it)->get_name();
		}
		std::string text = "bulk: " + ss.str();

		clear();

		return text;
	}

	std::chrono::time_point<std::chrono::system_clock> CommandPool::get_first_cmd_time_point() const {
		return first_command_time_;
	}
} // namespace otus
