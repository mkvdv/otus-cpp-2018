#include "two_tables_storage.h"

namespace otus {
  bool TwoTablesStorage::insertion(const std::string &table_name, int primary_key, std::string &&str) {
	  if (table_name == TABLE_NAME_A) {
		  return table_a_.insert(primary_key, table::simple_row_t{std::move(str)});
	  } else if (table_name == TABLE_NAME_B) {
		  return table_b_.insert(primary_key, table::simple_row_t{std::move(str)});
	  } else {
		  throw std::invalid_argument("Invalid table number");
	  }
  }

  void TwoTablesStorage::truncate(const std::string &table_name) {
	  if (table_name == TABLE_NAME_A) {
		  table_a_.truncate();
	  } else if (table_name == TABLE_NAME_B) {
		  table_b_.truncate();
	  } else {
		  throw std::invalid_argument("Invalid table number");
	  }
  }

  table::SimpleTable TwoTablesStorage::intersection() {
	  return table::intersection(table_a_, table_b_);
  }

  table::SimpleTable TwoTablesStorage::symmetric_difference() {
	  return table::symmetric_difference(table_a_, table_b_);
  }
} // namespace otus
