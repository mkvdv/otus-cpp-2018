#pragma once

#include <string>

namespace otus {
  /**
   * Special version of std::pair<string, size_t> for counting number of prefixes
   */
  class PrefixCounter {
   public:
	PrefixCounter() = delete;
	explicit PrefixCounter(std::string &&prefix);

	PrefixCounter(PrefixCounter &&) = default;
	PrefixCounter &operator=(PrefixCounter &&) = default;

	void add_count(size_t n) const;

	size_t count() const;

	size_t strlen() const;

	friend bool operator<(const PrefixCounter &, const PrefixCounter &);
	friend bool operator==(const PrefixCounter &, const PrefixCounter &);
	friend std::ostream &operator<<(std::ostream &, const PrefixCounter &);

   private:
	std::string prefix_;
	mutable size_t count_; // mutable cause add_count called from const ref in std::set container
  };

  bool operator<(const PrefixCounter &, const PrefixCounter &);
  bool operator==(const PrefixCounter &, const PrefixCounter &);
  bool operator!=(const PrefixCounter &, const PrefixCounter &);

  std::ostream &operator<<(std::ostream &, const PrefixCounter &);

} // namespace otus
