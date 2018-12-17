#include "hashing.h"

namespace otus {
  HashResult::HashResult(size_t size)
      : digest_(new unsigned char[size]), size_(size) {}

  HashResult::HashResult(HashResult &&other) noexcept {
      swap(other);
  }

  HashResult::HashResult(const HashResult &other) {
      size_ = other.size_;
      digest_ = new unsigned char[size_];
      std::copy(other.digest_, other.digest_ + size_, digest_);
  }

  HashResult &HashResult::operator=(HashResult &&other) noexcept {
      if (&other != this)
          swap(other);
      return *this;
  }

  HashResult::~HashResult() {
      delete[] digest_;
  }

  unsigned char *HashResult::digest() const {
      return digest_;
  }

  size_t HashResult::size() const {
      return size_;
  }

  void HashResult::swap(HashResult &other) noexcept {
      std::swap(digest_, other.digest_);
      std::swap(size_, other.size_);
  }

  bool operator==(const HashResult &lhs, const HashResult &rhs) {
      return lhs.size() == rhs.size()
          && std::char_traits<unsigned char>::compare(lhs.digest(), rhs.digest(), lhs.size()) == 0;
  }

  bool operator<(const HashResult &lhs, const HashResult &rhs) {
      return std::lexicographical_compare(lhs.digest(), lhs.digest() + lhs.size(),
                                          rhs.digest(), rhs.digest() + rhs.size());
  }

  /*--------------------------------------------------------------------------*/

  HashResult SHA1Hasher::hash(const unsigned char *data, size_t size) const {
      HashResult res(SHA_DIGEST_LENGTH);
      SHA1(data, size, res.digest());
      return res;
  }

  /*--------------------------------------------------------------------------*/

  HashResult MD5Hasher::hash(const unsigned char *data, size_t size) const {
      HashResult res(MD5_DIGEST_LENGTH);
      MD5(data, size, res.digest());
      return res;
  }
  /*--------------------------------------------------------------------------*/

  std::shared_ptr<IHasherImpl> make_hasher(std::string hash_algo_name) {
      if (hash_algo_name == "md5") {
          return std::make_shared<MD5Hasher>();
      } else if (hash_algo_name == "sha-1") {
          return std::make_shared<SHA1Hasher>();
      } else {
          throw std::runtime_error("unsupported hashing algorithm name");
      }
  }
} // namespace otus
