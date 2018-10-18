#include "../src/bulk_controller.h"
#include "../src/command/command_pool.h"
#include "../src/io/reader.h"
#include "../src/io/logger.h"

#include <gtest/gtest.h>

namespace {
	const size_t COMMANDS_PER_BLOCK = 3;

	const bool PRINT_INPUT_ENABLED = false;
}

class SmokeTestBulk: public ::testing::Test {
protected:
	void SetUp() override {
		input = std::make_unique<std::stringstream>();
		output = std::make_unique<std::stringstream>();
		expected = std::make_unique<std::stringstream>();

		empty_file_logger = std::make_unique<otus::EmptyFileLogger>();
		command_pool = std::make_unique<otus::CommandPool>();
		reader = std::make_unique<otus::Reader>(*input);
		bulk_logger = std::make_unique<otus::BulkLogger>(*output);

		controller = std::make_unique<otus::BulkController>(COMMANDS_PER_BLOCK,
		                                                    std::move(empty_file_logger),
		                                                    std::move(reader),
		                                                    std::move(bulk_logger),
		                                                    std::move(command_pool),
		                                                    PRINT_INPUT_ENABLED);

	}
	void TearDown() override {
		input.reset();
		output.reset();
		expected.reset();

		controller.reset();
		empty_file_logger.reset();
		command_pool.reset();
		reader.reset();
		bulk_logger.reset();
	}

	std::unique_ptr<std::stringstream> input;
	std::unique_ptr<std::stringstream> output;
	std::unique_ptr<std::stringstream> expected;

	std::unique_ptr<otus::BulkController> controller;
	std::unique_ptr<otus::IFileLogger> empty_file_logger;
	std::unique_ptr<otus::ICommandPool> command_pool;
	std::unique_ptr<otus::IReader> reader;
	std::unique_ptr<otus::IBulkLogger> bulk_logger;

};

TEST_F(SmokeTestBulk, cmd5) {
	*input << "cmd1\ncmd2\ncmd3\ncmd4\ncmd5\n";
	*expected << "\t| bulk: cmd1, cmd2, cmd3\n" << "\t| bulk: cmd4, cmd5\n";
	controller->start();

	ASSERT_EQ(output->str(), expected->str());
}

TEST_F(SmokeTestBulk, cmd7b) {
	*input << "cmd1\n"
	          "cmd2\n"
	          "cmd3\n"
	          "{\n"
	          "cmd4\n"
	          "cmd5\n"
	          "cmd6\n"
	          "cmd7\n"
	          "}";
	*expected << "\t| bulk: cmd1, cmd2, cmd3\n" << "\t| bulk: cmd4, cmd5, cmd6, cmd7\n";
	controller->start();

	ASSERT_EQ(output->str(), expected->str());
}

TEST_F(SmokeTestBulk, cmd_inner_block) {
	*input << "{\n"
	          "cmd1\n"
	          "cmd2\n"
	          "{\n"
	          "cmd3\n"
	          "cmd4\n"
	          "}\n"
	          "cmd5\n"
	          "cmd6\n"
	          "}";
	*expected << "\t| bulk: cmd1, cmd2, cmd3, cmd4, cmd5, cmd6\n";
	controller->start();

	ASSERT_EQ(output->str(), expected->str());
}

TEST_F(SmokeTestBulk, cmd_unclosed_block) {
	*input << "cmd1\n"
	          "cmd2\n"
	          "cmd3\n"
	          "{\n"
	          "cmd4\n"
	          "cmd5\n"
	          "cmd6\n"
	          "cmd7\n"
	          "";
	*expected << "\t| bulk: cmd1, cmd2, cmd3\n";
	controller->start();

	ASSERT_EQ(output->str(), expected->str());
}
