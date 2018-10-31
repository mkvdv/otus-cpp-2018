#include "../src/bulk_controller.h"
#include "../src/command/command_pool.h"
#include "../src/io/reader.h"

#include "../src/io/logger_job.h"
#include "../src/active/threadsafe_queue.h"
#include "../src/active/job_pool.h"
#include "../src/io/file_logger_job.h"
#include "../src/active/active_logger.h"

#include <iostream>
#include <sstream>

#include <gtest/gtest.h>

/**
 * CAUTION: Tests don't care about testing file logger. ¯\_(ツ)_/¯
 */

namespace {
	const size_t COMMANDS_PER_BLOCK = 3;
}

class SmokeTestBulk: public ::testing::Test {
protected:
	void SetUp() override {
		input = std::make_unique<std::stringstream>();
		logger_output_stream = std::make_unique<std::stringstream>();
		stats_output_stream = std::make_unique<std::stringstream>();
		expected = std::make_unique<std::stringstream>();

		logger_jobs = std::make_unique<otus::ThreadsafeQueue<otus::LoggerJob>>();
		activeLogger =
			std::make_unique<otus::ActiveLogger<otus::LoggerJob>>(*logger_jobs, *stats_output_stream);
		file_logger_jobs = std::make_unique<otus::ThreadsafeQueue<otus::FileLoggerJob>>();

		command_pool = std::make_unique<otus::CommandPool>();
		reader = std::make_unique<otus::Reader>(*input);
		controller = std::make_shared<otus::BulkController>(COMMANDS_PER_BLOCK,
		                                                    std::move(reader),
		                                                    std::move(command_pool),
		                                                    *logger_jobs,
		                                                    *file_logger_jobs,
		                                                    *logger_output_stream,
		                                                    *stats_output_stream);

	}

	void TearDown() override {
		controller.reset();
		reader.reset();
		command_pool.reset();

		file_logger_jobs.reset();
		activeLogger.reset();
		logger_jobs.reset();

		expected.reset();
		stats_output_stream.reset();
		logger_output_stream.reset();
		input.reset();
	}

	std::unique_ptr<std::stringstream> input;
	std::unique_ptr<std::stringstream> logger_output_stream;
	std::unique_ptr<std::stringstream> stats_output_stream;
	std::unique_ptr<std::stringstream> expected;

	std::unique_ptr<otus::ThreadsafeQueue<otus::LoggerJob>> logger_jobs;
	std::unique_ptr<otus::ActiveLogger<otus::LoggerJob>> activeLogger;

	std::unique_ptr<otus::ThreadsafeQueue<otus::FileLoggerJob>> file_logger_jobs;

	std::unique_ptr<otus::IReader> reader;
	std::unique_ptr<otus::ICommandPool> command_pool;
	std::shared_ptr<otus::BulkController> controller;
};

TEST_F(SmokeTestBulk, cmd5) {
	*input << "cmd1\ncmd2\ncmd3\ncmd4\ncmd5\n";
	*expected << "bulk: cmd1, cmd2, cmd3\n" << "bulk: cmd4, cmd5\n";

	controller->start();
	logger_jobs->stop_wait_and_block_pushing();
	file_logger_jobs->stop_wait_and_block_pushing();
	activeLogger->join();

	ASSERT_EQ(logger_output_stream->str(), expected->str());
}

TEST_F(SmokeTestBulk, one_more_block_test) {
	*input << "cmd1\n"
	          "cmd2\n"
	          "{\n"
	          "cmd3\n"
	          "cmd4\n"
	          "}\n"
	          "cmd5\n"
	          "cmd6\n";
	*expected << "bulk: cmd1, cmd2\n" << "bulk: cmd3, cmd4\n" << "bulk: cmd5, cmd6\n";

	controller->start();
	logger_jobs->stop_wait_and_block_pushing();
	file_logger_jobs->stop_wait_and_block_pushing();
	activeLogger->join();

	ASSERT_EQ(logger_output_stream->str(), expected->str());
}

TEST_F(SmokeTestBulk, one_block_test) {
	*input << "{\n"
	          "cmd1\n"
	          "cmd2\n"
	          "cmd3\n"
	          "cmd4\n"
	          "}\n";

	*expected << "bulk: cmd1, cmd2, cmd3, cmd4\n";

	controller->start();
	logger_jobs->stop_wait_and_block_pushing();
	file_logger_jobs->stop_wait_and_block_pushing();
	activeLogger->join();

	ASSERT_EQ(logger_output_stream->str(), expected->str());
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
	          "}\n";
	*expected << "bulk: cmd1, cmd2, cmd3\n" << "bulk: cmd4, cmd5, cmd6, cmd7\n";

	controller->start();
	logger_jobs->stop_wait_and_block_pushing();
	file_logger_jobs->stop_wait_and_block_pushing();
	activeLogger->join();

	ASSERT_EQ(logger_output_stream->str(), expected->str());
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
	          "}\n";
	*expected << "bulk: cmd1, cmd2, cmd3, cmd4, cmd5, cmd6\n";

	controller->start();
	logger_jobs->stop_wait_and_block_pushing();
	file_logger_jobs->stop_wait_and_block_pushing();
	activeLogger->join();

	ASSERT_EQ(logger_output_stream->str(), expected->str());
}

TEST_F(SmokeTestBulk, cmd_unclosed_block) {
	*input << "cmd1\n"
	          "cmd2\n"
	          "cmd3\n"
	          "{\n"
	          "cmd4\n"
	          "cmd5\n"
	          "cmd6\n"
	          "cmd7\n";
	*expected << "bulk: cmd1, cmd2, cmd3\n";

	controller->start();
	logger_jobs->stop_wait_and_block_pushing();
	file_logger_jobs->stop_wait_and_block_pushing();
	activeLogger->join();

	ASSERT_EQ(logger_output_stream->str(), expected->str());
}
