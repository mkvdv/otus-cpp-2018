#pragma once

#include "../hashing/hashing.h"

#include <boost/filesystem.hpp>

namespace otus {
  namespace fs = boost::filesystem;

  /**
   * Read file block by block, return hash of last readed block.
   */
  class ReadingState {
   public:
    ReadingState(fs::path &&p,
                 size_t file_size,
                 size_t block_size,
                 const std::shared_ptr<IHasherImpl> &hasher);
    ReadingState(ReadingState &&other) noexcept;
    ReadingState &operator=(ReadingState &&other) noexcept;

    HashResult read_block();
    fs::path path() const;
    bool is_end() const;

    void swap(ReadingState &other) noexcept;

   private:
    fs::path path_{};
    fs::ifstream ifs_{};
    size_t file_size_ = 0;
    size_t block_size_ = 0;
    size_t readed_size_ = 0;
    std::shared_ptr<IHasherImpl> hasher_;
  }; // class ReadingState

  bool operator<(const ReadingState &lhs, const ReadingState &rhs);

} // namespace otus
