#include "utils.h"

#include <boost/algorithm/string/replace.hpp>

namespace otus {
  void check_that_all_are_directories(const std::vector<fs::path> &dirs) {
      for (const fs::path &p: dirs) {
          fs::file_status st = fs::status(p);
          if (!fs::exists(p) || !fs::is_directory(st)) {
              throw std::runtime_error(p.generic_path().string() + " is not a directory");
          }
      }
  }

  bool fits_mask(const fs::path &path, const std::vector<boost::regex> &masks) {
      for (auto &mask: masks) {
          if (boost::regex_match(path.filename().string(), mask)) {
              return true;
          }
      }
      return false;
  }

  namespace {
    /**
     * Helper function for set_valid_pathes and set_valid_pathes_rec functions.
     *
     * Push path inside map [size -> valid pathes], if this path
     * fit mask and has appropriate size.
     * @param [out] valid_pathes
     * @param [in] masks
     * @param [in] pth
     * @param [in] min_size
     */
    void add_path_if_valid(std::map<size_t, std::set<fs::path>> &valid_pathes,
                           const std::vector<boost::regex> &masks,
                           const fs::path &pth, size_t min_size) {
        size_t size = fs::file_size(pth);
        if (size < min_size
            || (!masks.empty() && !fits_mask(pth, masks))) {
            return; // don't fit mask or size less than minimum
        }
        auto it = valid_pathes.find(size);
        if (it != valid_pathes.end()) {
            it->second.insert(pth);
        } else {
            valid_pathes[size] = std::set<fs::path>{pth};
        }
    }
  } // anonymous namespace

  void set_valid_pathes(std::map<size_t, std::set<fs::path>> &valid_pathes,
                        const fs::path &dir,
                        const std::vector<boost::regex> &masks,
                        size_t min_file_size) {
      fs::directory_iterator end;
      for (fs::directory_iterator dir_it(dir); dir_it != end; ++dir_it) {
          if (fs::is_regular_file(*dir_it)) {
              add_path_if_valid(valid_pathes, masks, dir_it->path(), min_file_size);
          }
      }
  }

  void set_valid_pathes_rec(std::map<size_t, std::set<fs::path>> &valid_pathes,
                            const fs::path &dir,
                            std::set<fs::path> &excluded_dirs,
                            const std::vector<boost::regex> &masks,
                            size_t min_size) {
      fs::recursive_directory_iterator end;
      for (fs::recursive_directory_iterator dir_it(dir); dir_it != end; ++dir_it) {
          if (fs::is_directory(*dir_it) && excluded_dirs.find(*dir_it) != excluded_dirs.end()) {
              dir_it.no_push(); // don't go recursively inside this directory
              continue;
          } else if (fs::is_regular_file(*dir_it) && fs::file_size(dir_it->path()) >= min_size) {
              add_path_if_valid(valid_pathes, masks, dir_it->path(), min_size);
          }
      }
  }

  std::vector<boost::regex> generate_regexes(std::vector<std::string> &&masks) {
      std::vector<boost::regex> res;
      res.reserve(masks.size());

      for (std::string &mask: masks) {
          boost::replace_all(mask, "*", ".*");
          boost::replace_all(mask, "?", ".?");
          res.emplace_back(std::move(mask), boost::regex::extended);
      }

      return res;
  }

} // namespace otus
