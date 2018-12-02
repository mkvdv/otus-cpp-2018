#include "find_prefix_function.h"

#include <set>
#include <algorithm>

#include <boost/log/trivial.hpp>

namespace otus {
  std::vector<otus::PrefixCounter> mapper(std::vector<std::string> &&lines) {
	  std::vector<otus::PrefixCounter> res;

	  for (const std::string &s: lines) {
		  for (size_t len = 1; len != s.size() + 1; ++len) {
			  res.emplace_back(s.substr(0, len));
		  }
	  }

	  return res;
  }

  otus::PrefixCounter reducer(std::vector<otus::PrefixCounter> &&prefixes) {
	  std::set<otus::PrefixCounter> count_prefixes; // will be sorted by length
	  {
		  auto prefixes_it = prefixes.begin();

		  while (prefixes_it != prefixes.end()) {
			  auto it = count_prefixes.find(*prefixes_it);
			  if (it != count_prefixes.end()) {
				  it->add_count(prefixes_it->count());
				  ++prefixes_it;
			  } else {
				  auto next_it = std::next(prefixes_it);
				  count_prefixes.insert(std::move(*prefixes_it));
				  prefixes_it = next_it;
			  }
		  }
	  }

	  // among all find shortest prefix that counts 1 times -- WRONG
	  // find longest string that counted > 1 times
	  class GreaterLen {
	   public:
		bool operator()(const otus::PrefixCounter &lhs, const otus::PrefixCounter &rhs) const {
			return lhs.strlen() > rhs.strlen();
		}
	  };
	  // sort in descending by length
	  std::vector<otus::PrefixCounter> desc_by_len;
	  desc_by_len.reserve(count_prefixes.size());

	  // move all from set to vector
	  auto it = count_prefixes.begin();
	  while (it != count_prefixes.end()) {
		  auto next_it = std::next(it);
		  desc_by_len.push_back(std::move(count_prefixes.extract(it).value()));
		  it = next_it;
	  }
	  std::sort(desc_by_len.begin(), desc_by_len.end(), GreaterLen{});

	  auto longest_not_uniq = std::find_if(
		  desc_by_len.begin(),
		  desc_by_len.end(),
		  [](const otus::PrefixCounter &obj) { return obj.count() > 1; });

	  if (longest_not_uniq == desc_by_len.end() || longest_not_uniq->count() == 1) {
		  return std::move(desc_by_len[0]);
	  } else {
		  BOOST_LOG_TRIVIAL(error) << "Reduced bunch to " << *longest_not_uniq;

		  return std::move(std::move(*longest_not_uniq));
	  }
  }
} // namespace otus
