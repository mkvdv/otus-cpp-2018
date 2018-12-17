#include "reading_state.h"

namespace otus {
  ReadingState::ReadingState(fs::path &&p,
                             size_t file_size,
                             size_t block_size,
                             const std::shared_ptr<IHasherImpl> &hasher)
      : path_(std::move(p)), ifs_(path_),
        file_size_(file_size), block_size_(block_size),
        hasher_(hasher) {}

  ReadingState::ReadingState(ReadingState &&other) noexcept {
      swap(other);
  }

  ReadingState &ReadingState::operator=(ReadingState &&other) noexcept {
      if (this != &other) {
          swap(other);
      }
      return *this;
  }

  void ReadingState::swap(ReadingState &other) noexcept {
      path_.swap(other.path_);
      ifs_.swap(other.ifs_);
      std::swap(file_size_, other.file_size_);
      std::swap(block_size_, other.block_size_);
      std::swap(readed_size_, other.readed_size_);
      std::swap(hasher_, other.hasher_);
  }

  HashResult ReadingState::read_block() {
      if (!is_end()) {
          std::unique_ptr<unsigned char[]> data = std::make_unique<unsigned char[]>(block_size_);
          long readed = ifs_.readsome(reinterpret_cast<char *>(data.get()), block_size_);
          readed_size_ += readed;
          return hasher_->hash(data.get(), static_cast<size_t>(readed));
      }
      throw std::runtime_error("is_end() return true, but u try to read");
  }

  fs::path ReadingState::path() const {
      return path_;
  }

  bool ReadingState::is_end() const {
      return readed_size_ >= file_size_;
  }

  bool operator<(const ReadingState &lhs, const ReadingState &rhs) {
      return lhs.path() < rhs.path();
  }
} // namespace otus
