#include "bulk_controller.h"
#include "command/command_pool.h"
#include "io/reader.h"
#include "io/logger.h"

#include <iostream>

int main(int argc, char *argv[]) {
	if (argc - 1 != 1) {
		std::cerr << "Invalid argument: expected one integral number";
		return EXIT_FAILURE;
	}

	size_t commands_per_block = std::stoull(argv[1]);
	const bool PRINT_INPUT = false;

	std::unique_ptr<otus::IFileLogger> file_logger = std::make_unique<otus::FileLogger>();
	std::unique_ptr<otus::IReader> reader = std::make_unique<otus::Reader>(std::cin);
	std::unique_ptr<otus::IBulkLogger> bulk_logger = std::make_unique<otus::BulkLogger>(std::cout);
	std::unique_ptr<otus::ICommandPool> command_pool = std::make_unique<otus::CommandPool>();;

	otus::BulkController controller(commands_per_block,
	                                std::move(file_logger),
	                                std::move(reader),
	                                std::move(bulk_logger),
	                                std::move(command_pool),
	                                PRINT_INPUT);
	controller.start();

	return EXIT_SUCCESS;
}
