#pragma once

#include "prefix_counter.h"

#include <vector>

namespace otus {
  // from vec of lines return list of suffixes
  std::vector<otus::PrefixCounter> mapper(std::vector<std::string> &&lines);

  // answer is the minimal length of some prefix, that counts 1 times
  // if some prefix counts 2 times -- this prefix is not enough
  otus::PrefixCounter reducer(std::vector<otus::PrefixCounter> &&prefixes);
} // namespace otus
