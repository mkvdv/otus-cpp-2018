#pragma once

#include "simple_table.h"

namespace otus {
  namespace {
	const char *TABLE_NAME_A = "A";
	const char *TABLE_NAME_B = "B";
  } // anonymous namespace

  /**
   * Contain two Simple tables with 2 columns {id, text}
   * Provide some operations.
   * Not threadsafe!
   */
  class TwoTablesStorage {
   public:
	TwoTablesStorage() = default;

	bool insertion(const std::string &table_name, int primary_key, std::string &&str);
	void truncate(const std::string &table_name);
	table::SimpleTable intersection();
	table::SimpleTable symmetric_difference();

   private:
	table::SimpleTable table_a_{TABLE_NAME_A, 1};
	table::SimpleTable table_b_{TABLE_NAME_B, 1};
  }; // class TwoTablesStorage
} // namespace otus
