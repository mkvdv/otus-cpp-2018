#include "bulk_controller.h"
#include "command/command_pool.h"
#include "io/reader.h"
#include "io/logger_job.h"
#include "io/file_logger_job.h"
#include "active/threadsafe_queue.h"
#include "active/job_pool.h"

#include <iostream>
#include <sstream>

namespace {
	using ActiveLogger = otus::JobPool<otus::LoggerJob, 1>;
}

int main(int argc, char *argv[]) {
	if (argc - 1 != 1) {
		std::cerr << "Invalid argument: expected one integral number";
		return EXIT_FAILURE;
	}

	size_t commands_per_block = std::stoull(argv[1]);

	std::istream &is = std::cin;
	std::ostream &logger_output_stream = std::cout;
	std::ostream &stats_stream = std::cout;

	/**
	 * Controller push LoggerJobs to logger_jobs queue, and in other thread ActiveLogger pop
	 * this jobs and make logging.
	 */
	otus::ThreadsafeQueue<otus::LoggerJob> logger_jobs;
	ActiveLogger loggerPool(logger_jobs, stats_stream, "logger_thread");

	/**
	 * Controller also push FileLoggerJobs to file_logger_jobs queue,
	 * and threads from file_logger_pool pop this jobs and write to file.
	 */
	otus::ThreadsafeQueue<otus::FileLoggerJob> file_logger_jobs;
	otus::JobPool<otus::FileLoggerJob, 4>
		file_logger_pool(file_logger_jobs, stats_stream, "file_logger_thread");

	std::unique_ptr<otus::IReader> reader = std::make_unique<otus::Reader>(is);
	std::unique_ptr<otus::ICommandPool> command_pool = std::make_unique<otus::CommandPool>();
	auto controller = std::make_shared<otus::BulkController>(commands_per_block,
	                                                         std::move(reader),
	                                                         std::move(command_pool),
	                                                         logger_jobs,
	                                                         file_logger_jobs,
	                                                         logger_output_stream,
	                                                         stats_stream);
	controller->start();
	// reading from input stream stopped here

	logger_jobs.stop_wait_and_block_pushing();      //  okay?
	loggerPool.join_all(); // wait active_logger read all queue and stopped, else output will mess with
	// stats

	file_logger_jobs.stop_wait_and_block_pushing();


	logger_output_stream << std::endl;

	return EXIT_SUCCESS;
}
