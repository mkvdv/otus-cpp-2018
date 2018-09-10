#pragma once

#include <cstddef>
#include <cstdint>

namespace {
  class S {
   public:
      S(int a, char b)
          : value_(static_cast<uint64_t>(a * b)) {}

      uint64_t get() const {
          return value_;
      }

   private:
      const uint64_t value_;
  };
} // anonymous namespace