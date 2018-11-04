#include <utility>

#include "bulk_controller.h"
#include "../command/command.h"


namespace otus {
	BulkController::BulkController(size_t commands_per_block,
	                               std::unique_ptr<IReader> reader,
	                               std::unique_ptr<ICommandPool> command_pool,
	                               otus::ThreadsafeQueue<LoggerJob> &logger_jobs,
	                               otus::ThreadsafeQueue<FileLoggerJob> &file_logger_jobs,
	                               std::ostream &logger_ostream,
	                               std::ostream &stats_stream,
	                               std::string prefix)
		: commands_per_block_(commands_per_block),
		  reader_(std::move(reader)),
		  pool_(std::move(command_pool)),
		  logger_jobs_(logger_jobs),
		  file_logger_jobs_(file_logger_jobs),
		  logger_ostream_(logger_ostream),
		  stats_stream_(stats_stream),
		  prefix_(std::move(prefix)) {}

	void BulkController::run_all_commands() {
		if (pool_->size()) {
			StatCounter diff_counter = current_counter_ - prev_jobstart_counter_;
			std::string text = pool_->run_all_commands_and_clear();

			file_logger_jobs_.push(FileLoggerJob(text, pool_->get_first_cmd_time_point(), diff_counter));
			logger_jobs_.push(LoggerJob(logger_ostream_, text, diff_counter, prefix_));

			prev_jobstart_counter_ = current_counter_;
		}
	}

	BulkController::~BulkController() {
		current_counter_.allow_strings();
		stats_stream_ << prefix_ << "Stats for Main thread:\t\t" << current_counter_ << std::endl;
	}

	void BulkController::start() {
		auto sptr = shared_from_this();

		std::weak_ptr<BulkController> weak_ptr(sptr);
		reader_->add_listener(weak_ptr);

		reader_->start_reading_cycle();

		/**
		 * Cause there can be some unexecuted commands
		 */
		if (depth_ == 0) {
			run_all_commands();
		}

		reader_->remove_listener(weak_ptr);
	}

	void BulkController::update(const std::string &s) {
		if (!s.empty()) {
			current_counter_.inc_str();
			if (s == "{") {
				if (depth_ == 0) {
					run_all_commands(); // flush all, if this is brand new block
					depth_ = 1;
				} else {
					depth_ += 1;
				}
			} else if (s == "}") {
				if (depth_ == 0) {
					throw std::runtime_error("invalid closed block without opened ones");
				} else {
					depth_ -= 1;
					if (depth_ == 0) {
						current_counter_.inc_blk();
						run_all_commands();
					}
				}
			} else {
				current_counter_.inc_cmd();
				pool_->add_command(std::make_unique<SimpleCommand>(s));
				if (depth_ == 0 && pool_->size() == commands_per_block_) {
					run_all_commands();
				}
			}
		}
	}

} // namespace otus
