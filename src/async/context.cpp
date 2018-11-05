#include <cassert>
#include <list>

#include "context.h"
#include "../io/reader.h"
#include "../command/command_pool.h"
#include "threadsafe_reader.h"

namespace otus::async {
	Context::Context(size_t bulk_size)
		: bulk_size_(bulk_size),
		  prefix_(std::string{"["} + std::to_string(reinterpret_cast<unsigned long long>(this)) + "] "),
		  active_logger_(logger_jobs_, std::cout, "logger_thread", prefix_),
		  file_logger_pool_(file_logger_jobs_, std::cout, "file_logger_thread", prefix_) {
		command_pool_ = std::make_unique<otus::CommandPool>();
		reader_ = std::make_unique<ThreadsafeReader>(queue_of_input_lines_);
		controller_ = std::make_shared<otus::BulkController>(bulk_size_,
		                                                     std::move(reader_),
		                                                     std::move(command_pool_),
		                                                     logger_jobs_,
		                                                     file_logger_jobs_,
		                                                     std::cout,
		                                                     std::cout,
		                                                     prefix_);
		worker_ = std::thread([this]() {
			controller_->start();

			//reading stopped here
			logger_jobs_.stop_wait_and_block_pushing();
			active_logger_.join_all();

			file_logger_jobs_.stop_wait_and_block_pushing();
			file_logger_pool_.join_all();
		});
	}

	Context::~Context() {
		worker_.join();
	}

	// private, helper function
	std::pair<std::list<std::string>, std::string> Context::split(std::string &&s) {
		std::list<std::string> splitted_lines;
		size_t beg_ix = 0;
		size_t end_ix;

		while ((end_ix = s.find_first_of('\n', beg_ix)) != std::string::npos) {
			if (beg_ix != end_ix) {
				splitted_lines.push_back(s.substr(beg_ix, end_ix - beg_ix));
			}
			beg_ix = end_ix + 1;
		}

		if (end_ix != std::string::npos && end_ix < s.length()) {
			return std::make_pair(splitted_lines, s.substr(end_ix));
		} else {
			return std::make_pair(splitted_lines, std::string{});
		}
	}

	void Context::input(const std::string &s) {

		if (size_t pos = s.find_last_of('\n'); pos != std::string::npos) {
			// concat with previous last chunk
			previosly_lost_str_ += s;

			// split for lines and push to queue
			std::list<std::string> splitted_lines;
			std::string nonterminatd_line;

			std::tie(splitted_lines, nonterminatd_line) = split(std::move(previosly_lost_str_));

			// move to queue
			for (std::string &line : splitted_lines) {
				queue_of_input_lines_.push(std::move(line));
			}

			previosly_lost_str_ = nonterminatd_line;
		} else {
			previosly_lost_str_ += s;
		}
	}

	void Context::block_queue_input_and_stop_waiters() {
		queue_of_input_lines_.stop_wait_and_block_pushing();
	}

} // namespace otus::async
