#pragma once

#include <vector>
#include <string>
#include <boost/filesystem.hpp>

namespace otus {
  namespace fs = boost::filesystem;

  /**
   * Find duplicated files in specified directories.
   *
   * @param dirs vector of input directories
   * @param ex_dirs excluded directories
   * @param recursive scanning level
   * @param min_size minimum checking files size
   * @param masks masks for files
   * @param bsize block_size for reading files
   * @param hash_name name of hashing algorithm
   * @return vector of groups of pathes to duplicated files -- files in every group are equal
   */
  std::vector<std::vector<fs::path>>
  find_duplicates(std::vector<fs::path> &&dirs,
                  std::vector<fs::path> &&ex_dirs,
                  bool recursive, size_t min_size,
                  std::vector<std::string> &&string_masks,
                  size_t bsize,
                  const std::string &hash_name);
} // namespace otus
