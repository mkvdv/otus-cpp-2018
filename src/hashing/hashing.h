#pragma once

#include <string>
#include <memory>

#include <openssl/sha.h>
#include <openssl/md5.h>

namespace otus {
  /**
   * Result of hashing some data.
   * C++ wrapper over unsigned char *.
   */
  class HashResult {
   public:
    explicit HashResult(size_t size);

    HashResult() = default;

    HashResult(HashResult &&other) noexcept;
    HashResult &operator=(HashResult &&other) noexcept;
    HashResult(const HashResult &other);
    ~HashResult();

    void swap(HashResult &other) noexcept;
    unsigned char *digest() const;
    size_t size() const;

   private:
    unsigned char *digest_ = nullptr;
    size_t size_ = 0;
  };

  bool operator<(const HashResult &lhs, const HashResult &rhs);

  bool operator==(const HashResult &lhs, const HashResult &rhs);

  class IHasherImpl {
   public:
    virtual HashResult hash(const unsigned char *data, size_t size) const = 0;
    virtual ~IHasherImpl() = default;
  };

  class SHA1Hasher : public IHasherImpl {
   public:
    HashResult hash(const unsigned char *data, size_t size) const override;
  };

  class MD5Hasher : public IHasherImpl {
   public:
    HashResult hash(const unsigned char *data, size_t size) const override;
  };

  std::shared_ptr<IHasherImpl> make_hasher(std::string hash_algo_name);

} // namespace otus
