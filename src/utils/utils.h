#pragma once

#include <vector>
#include <set>
#include <map>

#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

namespace otus {
  template<typename ContainerT, typename PredicateT>
  void erase_if(ContainerT &items, const PredicateT &predicate) {
      for (auto it = items.begin(); it != items.end();) {
          if (predicate(*it)) it = items.erase(it);
          else ++it;
      }
  }

  namespace fs = boost::filesystem;

  /**
   * Check that all pathes in input vector are directories.
   * THROW std::runtime_error in case of detecting not directory
   */
  void check_that_all_are_directories(const std::vector<fs::path> &dirs);

  bool fits_mask(const fs::path &path, const std::vector<boost::regex> &masks);

  /**
   * Traverse specified directory and set only valid pathes
   * (which meet the requirements) to valid_pathes  map
   *
   * @param [out] valid_pathes
   * @param [in] dir
   * @param [in] masks
   * @param [in] min_file_size
   */
  void set_valid_pathes(std::map<size_t, std::set<fs::path>> &valid_pathes,
                        const fs::path &dir,
                        const std::vector<boost::regex> &masks,
                        size_t min_file_size);

  /**
   * Recursively traverse specified directory and set only valid pathes
   * (which meet the requirements) to valid_pathes  map
   *
   * @param [out] valid_pathes
   * @param [in] dir
   * @param [in] masks
   * @param [in] min_file_size
   */
  void set_valid_pathes_rec(std::map<size_t, std::set<fs::path>> &valid_pathes,
                            const fs::path &dir,
                            std::set<fs::path> &excluded_dirs,
                            const std::vector<boost::regex> &masks,
                            size_t min_size);

  /**
   * From vec of boost::regex from file masks like fi?e_name.* or something else
   * generate PCRE regular expressions.
   */
  std::vector<boost::regex> generate_regexes(std::vector<std::string> &&masks);

} // namespace otus
