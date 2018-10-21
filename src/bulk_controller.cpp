#include "bulk_controller.h"
#include "command/command.h"

namespace otus {
	BulkController::BulkController(size_t commands_per_block,
	                               std::unique_ptr<IFileLogger> file_logger,
	                               std::unique_ptr<IReader> reader,
	                               std::unique_ptr<IBulkLogger> bulk_logger,
	                               std::unique_ptr<ICommandPool> command_pool,
	                               bool print_input_enabled)
		: commands_per_block_(commands_per_block),
		  file_logger_(std::move(file_logger)),
		  reader_(std::move(reader)),
		  bulk_logger_(std::move(bulk_logger)),
		  pool_(std::move(command_pool)),
		  print_input_enabled_(print_input_enabled) {
		reader_->add_listener(this);
	}

	void BulkController::run_all_commands() {
		if (pool_->size()) {
			std::string text = pool_->run_all_commands_and_clear();

			bulk_logger_->log_output(text);
			file_logger_->write_to_file(pool_->get_first_cmd_time_point(), text);
		}
	}

	void BulkController::start() {
		reader_->start_reading_cycle();

		/**
		 * Cause there can be some unexecuted commands
		 */
		if (depth_ == 0) {
			run_all_commands();
		}
	}

	void BulkController::update(const std::string &s) {
		if (!s.empty()) {
			if (print_input_enabled_) {
				bulk_logger_->log_input(s);
			}

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
						run_all_commands();
					}
				}
			} else {
				pool_->add_command(std::make_unique<SimpleCommand>(s));
				if (depth_ == 0 && pool_->size() == commands_per_block_) {
					run_all_commands();
				}
			}
		}
	}

} // namespace otus
