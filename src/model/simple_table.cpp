#include "simple_table.h"

namespace otus::table {
  table::SimpleTable::SimpleTable(std::string table_name, size_t row_length) : table_name_(std::move(
	  table_name)), row_length_(row_length) {}

  bool SimpleTable::insert(int primary_key, const simple_row_t &value) {
	  if (value.size() != row_length_) {
		  throw std::invalid_argument("Wrong number of columns");
	  }
	  auto it = storage_.find(primary_key);
	  if (it != storage_.end()) {
		  return false;
	  } else {
		  storage_[primary_key] = value;
		  return true;
	  }
  }
  void SimpleTable::update(int primary_key, const simple_row_t &value) {
	  if (value.size() != row_length_) {
		  throw std::invalid_argument("Wrong number of columns");
	  }
	  storage_[primary_key] = value;
  }
  void SimpleTable::truncate() {
	  storage_.clear();
  }

  std::string SimpleTable::name() const {
	  return table_name_;
  }
  size_t SimpleTable::size() const {
	  return storage_.size();
  }
  size_t SimpleTable::row_length() const {
	  return row_length_;
  }
  bool SimpleTable::empty() const {
	  return size() == 0;
  }

  std::map<int, simple_row_t>::const_iterator SimpleTable::cbegin() const {
	  return storage_.cbegin();
  }
  std::map<int, simple_row_t>::const_iterator SimpleTable::cend() const {
	  return storage_.cend();
  }

  std::ostream &operator<<(std::ostream &os, const simple_row_t &vec) {
	  for (const std::string &s : vec) {
		  if (!s.empty()) {
			  os << "," << s;
		  } else {
			  os << ",";
		  }
	  }
	  return os;
  }
  std::ostream &operator<<(std::ostream &os, const SimpleTable &table) {
	  for (auto it = table.cbegin(); it != table.cend(); ++it) {
		  os << it->first << it->second << "\n";
	  }
	  return os;
  }

  SimpleTable intersection(const SimpleTable &lhs, const SimpleTable &rhs) {
	  SimpleTable res("(" + lhs.name() + ") ∩ (" + rhs.name() + ")", lhs.row_length() + rhs.row_length());

	  auto left_it = lhs.cbegin();
	  auto right_it = rhs.cbegin();

	  // keys are Sorted, so we can do it fast // aka index =)
	  while (left_it != lhs.cend() && right_it != rhs.cend()) {
		  if (left_it->first < right_it->first) {
			  ++left_it;
		  } else if (left_it->first > right_it->first) {
			  ++right_it;
		  } else {
			  simple_row_t new_row = left_it->second;
			  std::copy(right_it->second.begin(), right_it->second.end(), std::back_inserter(new_row));
			  res.update(left_it->first, new_row);

			  ++left_it;
			  ++right_it;
		  }
	  }

	  return res;
  }

  SimpleTable symmetric_difference(const SimpleTable &lhs, const SimpleTable &rhs) {
	  SimpleTable res("(" + lhs.name() + ") ∆ (" + rhs.name() + ")", lhs.row_length() + rhs.row_length());

	  auto left_it = lhs.cbegin();
	  auto right_it = rhs.cbegin();

	  /**
	   * helper lambdas, used 2 times, exists cause DRY
	   */
	  auto add_row_with_empty_left = [&right_it, &lhs, &res]() {
		simple_row_t new_row;
		for (size_t i = 0; i != lhs.row_length(); ++i) {
			new_row.emplace_back("");
		}
		std::copy(right_it->second.begin(), right_it->second.end(), std::back_inserter(new_row));

		res.update(right_it->first, new_row);

	  };

	  auto add_row_with_empty_right = [&left_it, &rhs, &res]() {
		simple_row_t new_row = left_it->second;
		for (size_t i = 0; i != rhs.row_length(); ++i) {
			new_row.emplace_back("");
		}

		res.update(left_it->first, new_row);
	  };

	  /**
	   * keys are Sorted, so we can do it O(n)
	   */
	  while (left_it != lhs.cend() && right_it != rhs.cend()) {
		  if (left_it->first < right_it->first) {
			  add_row_with_empty_right();
			  ++left_it;
		  } else if (left_it->first > right_it->first) {
			  add_row_with_empty_left();
			  ++right_it;
		  } else { // skip equal
			  ++left_it;
			  ++right_it;
		  }
	  }

	  // add tail of one of table
	  while (left_it != lhs.cend()) {
		  add_row_with_empty_right();
		  ++left_it;
	  }
	  while (right_it != rhs.cend()) {
		  add_row_with_empty_left();
		  ++right_it;
	  }

	  return res;
  }
} // namespace otus::table
