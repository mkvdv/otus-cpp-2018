#include "prefix_counter.h"
#include <cassert>

namespace otus {
  PrefixCounter::PrefixCounter(std::string &&prefix)
	  : prefix_(prefix), count_(1) {
	  assert(!prefix_.empty());
  }

  void PrefixCounter::add_count(size_t n) const {
	  count_ += n;
  }

  size_t PrefixCounter::count() const {
	  return count_;
  }

  size_t PrefixCounter::strlen() const {
	  return prefix_.length();
  }

  bool operator<(const PrefixCounter &lhs, const PrefixCounter &rhs) {
	  return lhs.prefix_ < rhs.prefix_;
  }
  bool operator==(const PrefixCounter &lhs, const PrefixCounter &rhs) {
	  return lhs.prefix_ == rhs.prefix_;
  }
  bool operator!=(const PrefixCounter &lhs, const PrefixCounter &rhs) {
	  return !(lhs == rhs);
  }
  std::ostream &operator<<(std::ostream &os, const PrefixCounter &obj) {
	  os << obj.prefix_;
	  return os;
  }
} // namespace otus
