#pragma once

#include <cstddef>
#include <ostream>

namespace otus {
	class StatCounter {
	public:
		StatCounter() = default;
		StatCounter(StatCounter &&) = default;
		StatCounter(const StatCounter &) = default;
		StatCounter &operator=(const StatCounter &) = default;

		StatCounter(size_t string_counter, size_t commands_couter_, size_t block_counter_);

		void inc_str();
		void inc_cmd();
		void inc_blk();
		void allow_strings();

		StatCounter &operator+=(const StatCounter &other);
		StatCounter &operator-=(const StatCounter &other);

		friend std::ostream &operator<<(std::ostream &, const StatCounter &);

	private:
		size_t string_counter_ = 0;
		size_t commands_couter_ = 0;
		size_t block_counter_ = 0;
		bool allow_strings_ = false;
	};

	inline std::ostream &operator<<(std::ostream &os, const StatCounter &cnt) {
		if (cnt.allow_strings_) {
			os << cnt.string_counter_ << " strings, ";
		}
		os << cnt.commands_couter_ << " commands, " << cnt.block_counter_ << " blocks";
		return os;
	}

	inline StatCounter operator+(StatCounter lhs, const StatCounter &rhs) {
		lhs += rhs;
		return lhs;
	}

	inline StatCounter operator-(StatCounter lhs, const StatCounter &rhs) {
		lhs -= rhs;
		return lhs;
	}

} // namespace otus
