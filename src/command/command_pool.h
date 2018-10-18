#pragma once

#include "icommand_pool.h"

#include <vector>

namespace otus {
	class CommandPool: public ICommandPool {
	public:
		void add_command(std::unique_ptr<ICommand> &&new_cmd) override;
		size_t size() const override;
		void clear() override;
		std::string run_all_commands_and_clear() override;
		std::chrono::time_point<std::chrono::system_clock> get_first_cmd_time_point() const override;

		~CommandPool() override = default;
	private:
		std::vector<std::unique_ptr<ICommand>> commands_to_exec_{};
		std::chrono::time_point<std::chrono::system_clock> first_command_time_{};
	};

} // namespace otus
