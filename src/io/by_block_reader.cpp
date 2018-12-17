#include "by_block_reader.h"

#include <utility>
#include <memory>
#include <iostream>

//#define DEBUG

namespace otus {
  ByBlockReader::ByBlockReader(std::map<size_t, std::set<fs::path>> &&valid_pathes,
                               size_t bsize,
                               const std::shared_ptr<IHasherImpl> &hasher) {
      for (auto &size_n_set_of_pathes:  valid_pathes) {
          std::set<fs::path> &set_of_pathes = size_n_set_of_pathes.second;
          size_t size = size_n_set_of_pathes.first;

          for (auto it = set_of_pathes.begin(); it != set_of_pathes.end(); /* */ ) {
              auto next = std::next(it);
              auto node = set_of_pathes.extract(it);

              ReadingState reading_state(std::move(node.value()), size, bsize, hasher);
              HashResult hash_result = reading_state.read_block(); // first read
              insert(size, std::move(hash_result), std::move(reading_state));
              DEBUG_print_storage("Inserted");

              it = next;
          }
      }
      DEBUG_print_storage("Constructed!");
  }

  bool ByBlockReader::is_empty() const {
      return size_to_hash_n_readers_.empty();
  }

  void ByBlockReader::read_next_block(std::vector<std::vector<fs::path>> &duplicate_groups) {
      DEBUG_print_storage("Before clear and save");
      clear_n_save_duplicates(duplicate_groups);
      DEBUG_print_storage("Cleared");

      std::map<size_t, hashing_map_t> moved = std::move(size_to_hash_n_readers_);
      size_to_hash_n_readers_ = std::map<size_t, hashing_map_t>{};

      for (auto &size_to_maps:  moved) {
          hashing_map_t &map_ref = size_to_maps.second;
          size_t size = size_to_maps.first;

          for (auto it = map_ref.begin(); it != map_ref.end(); /* */ ) {
              std::set<ReadingState> &set_ref = it->second;
              auto next = std::next(it);

              // otherwise read next block from this set
              read_block_and_extract(set_ref, size);
              if (set_ref.empty()) {
                  map_ref.erase(it);
              }
              it = next;
          }
      }

      DEBUG_print_storage("Readed next block");
  }

  /* ________________________________ private ______________________________ */

  void ByBlockReader::read_block_and_extract(std::set<ReadingState> &set_ref, size_t size) {
      for (auto it = set_ref.begin(); it != set_ref.end();  /* */ ) {
          auto next = std::next(it);
          auto node = set_ref.extract(it);

          ReadingState extracted = std::move(node.value());
          HashResult new_hash = extracted.read_block();
          insert(size, std::move(new_hash), std::move(extracted));

          it = next;
      }
  }

  void ByBlockReader::insert(size_t size, HashResult &&hash_res, ReadingState &&reading_state) {
      auto it = size_to_hash_n_readers_.find(size);

      if (it != size_to_hash_n_readers_.end()) {      // found
          hashing_map_t &hashing_map_ref = it->second;
          auto hash_to_read_it = hashing_map_ref.find(hash_res);
          if (hash_to_read_it != hashing_map_ref.end()) { // this hash was already, set exist
              std::set<ReadingState> &set_ref = hash_to_read_it->second;
              auto p = set_ref.insert(std::move(reading_state));
              if (!p.second) {
                  throw std::logic_error("Insertion to set wit no success, dublicate pathes?");
              }

          } else {
              std::set<ReadingState> new_set;
              new_set.insert(std::move(reading_state));
              hashing_map_ref.insert(std::make_pair(std::move(hash_res), std::move(new_set)));
          }

      } else {                                        // not found
          std::set<ReadingState> new_set;
          new_set.insert(std::move(reading_state));
          hashing_map_t new_map;
          new_map.insert(std::make_pair(std::move(hash_res), std::move(new_set)));
          size_to_hash_n_readers_.insert(std::make_pair(size, std::move(new_map)));
      }
  }

  void ByBlockReader::clear_n_save_duplicates(std::vector<std::vector<fs::path>> &duplicate_groups) {
      for (auto size_to_map_it = size_to_hash_n_readers_.begin();
           size_to_map_it != size_to_hash_n_readers_.end();
          /** */ ) {
          hashing_map_t &map_ref = size_to_map_it->second;
          auto size_to_map_next_it = std::next(size_to_map_it);

          for (auto hash_to_set_it = map_ref.begin(); hash_to_set_it != map_ref.end(); /* */) {
              std::set<ReadingState> &set_ref = hash_to_set_it->second;
              auto next = std::next(hash_to_set_it);

              if (set_ref.size() <= 1) {
                  // if size of set == 1 -> no duplicates, drop this set
                  map_ref.erase(hash_to_set_it);

              } else if (set_ref.begin()->is_end()) {
                  // check first reading_state - others in this group
                  // if size of set > 1 AND any reading_state is_end == false - this is duplicate
                  // has read the same shit
                  std::vector<fs::path> duplicates;
                  duplicates.reserve(set_ref.size());
                  for (const ReadingState &rs: set_ref) {
                      duplicates.push_back(rs.path());
                  }

                  duplicate_groups.push_back(std::move(duplicates)); // save to result
                  map_ref.erase(hash_to_set_it); // remove this set from executed
              }

              hash_to_set_it = next;
          }

          if (map_ref.empty()) {
              size_to_hash_n_readers_.erase(size_to_map_it);
          }

          size_to_map_it = size_to_map_next_it;
      }
  }

  void ByBlockReader::DEBUG_print_storage(const char *s) {
      (void) s;

#ifdef DEBUG
      std::cout << s << '\n';

      for (auto &size_to_maps:  size_to_hash_n_readers_) {
          hashing_map_t &map_ref = size_to_maps.second;
          size_t size = size_to_maps.first;

          for (auto &hash_to_set : map_ref) {
              std::set<ReadingState> &set_ref = hash_to_set.second;
              for (const ReadingState &reading_state : set_ref) {
                  std::cout << size << ", " << reading_state.path() << '\n';
              }
          }
      }

      std::cout << '\n';
#endif
  }
} // namespace otus
