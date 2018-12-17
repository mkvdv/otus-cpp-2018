#include "finder.h"
#include "hashing/hashing.h"
#include "utils/utils.h"
#include "io/reading_state.h"
#include "io/by_block_reader.h"

#include <queue>

namespace otus {
  std::vector<std::vector<fs::path>>
  find_duplicates(std::vector<fs::path> &&dirs,
                  std::vector<fs::path> &&ex_dirs,
                  bool recursive, size_t min_size,
                  std::vector<std::string> &&string_masks,
                  size_t bsize,
                  const std::string &hash_name) {
      std::vector<std::vector<fs::path>> duplicate_group_of_pathes;

      // check that input directories is valid
      check_that_all_are_directories(dirs);
      check_that_all_are_directories(ex_dirs);
      std::vector<boost::regex> masks = generate_regexes(std::move(string_masks));

      std::shared_ptr<IHasherImpl> hasher = make_hasher(hash_name);
      assert(hasher);

      std::queue<fs::path> dirs_to_process(
          std::deque<fs::path>{std::make_move_iterator(dirs.begin()),
                               std::make_move_iterator(dirs.end())}
      );

      std::set<fs::path> excluded_dirs(std::make_move_iterator(ex_dirs.begin()),
                                       std::make_move_iterator(ex_dirs.end()));

      std::map<size_t, std::set<fs::path>> valid_pathes; // map from file_size to set of pathes
      // traverse directories and get valid pathes

      while (!dirs_to_process.empty()) {
          fs::path dir = std::move(dirs_to_process.front());
          dirs_to_process.pop();

          if (excluded_dirs.find(dir) == excluded_dirs.end()) {
              if (recursive) {
                  set_valid_pathes_rec(valid_pathes, dir, excluded_dirs, masks, min_size);
              } else {
                  set_valid_pathes(valid_pathes, dir, masks, min_size);
              }
          }
      }

      // remove all sets with just one path - cause it is definitely not duplicate
      erase_if(valid_pathes, [](decltype(valid_pathes)::value_type &size_to_set) {
          return size_to_set.second.size() <= 1;
      });

      // { size_of_file -> { HashResult -> [ReadingState] } }
      ByBlockReader file_readers(std::move(valid_pathes), bsize, hasher);

      while (!file_readers.is_empty()) {
          file_readers.read_next_block(duplicate_group_of_pathes);
      }

      return duplicate_group_of_pathes;
  }
} // namespace otus
