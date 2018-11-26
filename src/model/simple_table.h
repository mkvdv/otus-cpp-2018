#pragma once

#include <utility>
#include <string>
#include <map>
#include <iostream>
#include <optional>

#include <cstddef>
#include <vector>

namespace otus::table {
  using simple_row_t = std::vector<std::string>;

  /**
   * Every table's row contain integer id in first column (primary key) and row_length string fields.
   * This class provide some operations on table.
   * Not threadsafe!
   */
  class SimpleTable {
   public:
	explicit SimpleTable(std::string table_name, size_t row_length);

	/**
	 * Insert value only if key not exist.
	 *
	 * @return true on success, false otherwise
	 */
	bool insert(int primary_key, const simple_row_t &value);

	/**
	 * Change value for primary_key, overwrite it if need this.
	 */
	void update(int primary_key, const simple_row_t &value);
	void truncate();

	std::string name() const;

	/**
	 * @return number of rows
	 */
	size_t size() const;

	size_t row_length() const;
	bool empty() const;

	std::map<int, simple_row_t>::const_iterator cbegin() const;
	std::map<int, simple_row_t>::const_iterator cend() const;

   private:
	std::string table_name_;
	std::map<int, simple_row_t> storage_;
	size_t row_length_;
  }; // class SimpleTable

  /**
   * Print all stuff to stream.
   */
  std::ostream &operator<<(std::ostream &os, const simple_row_t &vec);
  std::ostream &operator<<(std::ostream &os, const SimpleTable &table);

  SimpleTable intersection(const SimpleTable &lhs, const SimpleTable &rhs);
  SimpleTable symmetric_difference(const SimpleTable &lhs, const SimpleTable &rhs);
} // namespace otus::table
