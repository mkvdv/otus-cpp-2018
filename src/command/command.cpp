#include "command.h"

namespace otus {
	SimpleCommand::SimpleCommand(std::string name)
		: name_(std::move(name)) {}

	void SimpleCommand::exec() {
		/**
		 * Do nothing!
		 */
	}

	std::string SimpleCommand::get_name() const {
		return name_;
	}
} // namespace otus
