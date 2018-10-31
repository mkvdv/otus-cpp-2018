#include "counter.h"

namespace otus {
	StatCounter::StatCounter(size_t string_counter, size_t commands_couter_, size_t block_counter_)
		: string_counter_(string_counter),
		  commands_couter_(commands_couter_),
		  block_counter_(block_counter_) {}

	void StatCounter::inc_str() {
		string_counter_ += 1;
	}

	void StatCounter::inc_cmd() {
		commands_couter_ += 1;
	}

	void StatCounter::inc_blk() {
		block_counter_ += 1;
	}

	void StatCounter::allow_strings() {
		allow_strings_ = true;
	}

	StatCounter &StatCounter::operator+=(const StatCounter &other) {
		string_counter_ += other.string_counter_;
		commands_couter_ += other.commands_couter_;
		block_counter_ += other.block_counter_;

		return *this;
	}
	StatCounter &StatCounter::operator-=(const StatCounter &other) {
		string_counter_ -= other.string_counter_;
		commands_couter_ -= other.commands_couter_;
		block_counter_ -= other.block_counter_;

		return *this;
	}

} // namespace otus

