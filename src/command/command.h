#pragma once

#include "icommand.h"

namespace otus {
	class SimpleCommand: public ICommand {
	public:
		explicit SimpleCommand(std::string name);
		void exec() override;
		std::string get_name() const override;

	private:
		std::string name_;
	};

} // namespace otus
