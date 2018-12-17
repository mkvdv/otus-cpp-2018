#pragma once

#include "../hashing/hashing.h"
#include "reading_state.h"

#include <map>
#include <set>

namespace otus {
  /**
   * Contain all File readers, which read for 1 block from file at once.
   * Manage all them, provide function to read next block and save
   * some result to outside source.
   */
  class ByBlockReader {
   public:
    using hashing_map_t = std::map<HashResult, std::set<ReadingState>>;

    explicit ByBlockReader(std::map<size_t, std::set<fs::path>> &&valid_pathes,
                           size_t bsize,
                           const std::shared_ptr<IHasherImpl> &hasher);

    /**
     * @return true if all readers already done or and all duplicates detected
     */
    bool is_empty() const;
    // make a copy, insert back with reading like a new hash
    void read_next_block(std::vector<std::vector<fs::path>> &duplicate_groups);

   private:
    void read_block_and_extract(std::set<ReadingState> &set_ref, size_t size);
    void insert(size_t size, HashResult &&hash_res, ReadingState &&reading_state);

    /**
     * Remove definitely not duplicates from storage,
     * and save to duplicate_groups vector new defenitely duplicates
     * The reason
     * @param [out] duplicate_groups
     */
    void clear_n_save_duplicates(std::vector<std::vector<fs::path>> &duplicate_groups);
    void DEBUG_print_storage(const char *s);

   private:
    std::map<size_t, hashing_map_t> size_to_hash_n_readers_{};
  };
} // namespace otus
