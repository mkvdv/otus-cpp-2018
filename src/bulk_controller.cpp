#include "bulk_controller.h"
#include "command/command.h"

namespace otus {
  BulkController::BulkController(size_t commands_per_block,
                                 std::unique_ptr<IFileLogger> file_logger,
                                 std::unique_ptr<IReader> reader,
                                 std::unique_ptr<IBulkLogger> bulk_logger,
                                 std::unique_ptr<ICommandPool> command_pool)
      : commands_per_block_(commands_per_block),
        file_logger_(std::move(file_logger)),
        reader_(std::move(reader)),
        bulk_logger_(std::move(bulk_logger)), pool_(std::move(command_pool)) {}

  void BulkController::run_all_commands() {
      if (pool_->size()) {
          // first -- prepare text, than add with this text writting ti console job
          // and writing to file job
          std::vector<std::string> results = pool_->run_all_commands_and_clear();

          bulk_logger_->log_output(results);
          file_logger_->write_to_file(pool_->get_first_cmd_time_point(), results);
      }
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

} // namespace otus
