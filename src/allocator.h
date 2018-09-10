#pragma once

#include "fixed_size_list.h"

#include <new>
#include <list>
#include <cstddef>
#include <limits>
#include <memory>
#include <iostream>

//#define DEBUG

namespace otus {
/**
 * Simple caching allocator
 */
  template<class T, std::size_t CACHE_SIZE>
  class otus_allocator {
   public:
      using value_type = T;
      using pointer = T *;
      using const_pointer = const T *;
      using reference = T &;
      using const_reference = const T &;
      using size_type = std::size_t;
      using difference_type = std::ptrdiff_t;

      template<class U>
      struct rebind { typedef otus_allocator<U, CACHE_SIZE> other; };

      otus_allocator() = default;
      otus_allocator(otus_allocator &&) noexcept = default;
      otus_allocator &operator=(otus_allocator &&) noexcept = default;
      ~otus_allocator() = default;

      // nocopyable
      otus_allocator(const otus_allocator &) = delete;
      otus_allocator &operator=(const otus_allocator &) = delete;

      pointer address(reference x) const;
      const_pointer address(const_reference x) const;

      pointer allocate(size_type n, const void *hint = nullptr);
      void deallocate(pointer p, size_type n);

      void construct(pointer p, const_reference val);
      template<class U, class... Args>
      void construct(U *p, Args &&... args);

      void destroy(pointer p);
      template<class U>
      void destroy(U *p);

      size_type max_size() const;

   private:
      std::list<fixed_size_list<T, CACHE_SIZE>> caches_;
  };

  template<class T, size_t CACHE_SIZE>
  typename otus_allocator<T, CACHE_SIZE>::pointer
  otus_allocator<T, CACHE_SIZE>::address(reference x)
  const {
      return &x;
  }

  template<class T, size_t CACHE_SIZE>
  typename otus_allocator<T, CACHE_SIZE>::const_pointer
  otus_allocator<T, CACHE_SIZE>::address(const_reference x) const {
      return &x;
  }

  template<class T, size_t CACHE_SIZE>
  typename otus_allocator<T, CACHE_SIZE>::pointer
  otus_allocator<T, CACHE_SIZE>::allocate(otus_allocator::size_type n, const void *hint) {
      (void) hint;
      using namespace std;
      // Пока так, достать из кеша можно только один объект (иначе идея с таким списком не зайдет
      // и надо будет boarder tags или опять slab allocator писать
      if (n == 1) {
          T *res = nullptr;
          for (fixed_size_list<T, CACHE_SIZE> &cache: caches_) {
              if (!cache.filled()) {
                  res = cache.alloc();
              }
          }
          if (!res) {
              caches_.push_front(fixed_size_list<T, CACHE_SIZE>{});
              res = caches_.front().alloc();
          }
#ifdef DEBUG
          cout << "# From cache got 1 elem of size " << sizeof(T) << " bytes" << endl;
#endif
          return res;
      } else {
          #ifdef DEBUG
          cout << "From ::operator new got" << n << "elems of size "
               << sizeof(T) << " bytes" << endl;
          #endif
          return (pointer) ::operator new(n * sizeof(T));
      }
  }

  template<class T, size_t CACHE_SIZE>
  void otus_allocator<T, CACHE_SIZE>::deallocate(otus_allocator::pointer p,
                                                 otus_allocator::size_type n) {
      using namespace std;
      if (n == 1) {
          for (fixed_size_list<T, CACHE_SIZE> &cache: caches_) {
              if (cache.contain_addr(p)) {
                  cache.dealloc(p);
#ifdef DEBUG
                  cout << "# To cache returned 1 elem of size " << sizeof(T) << " bytes" << endl;
#endif
                  return;
              }
          }
          throw runtime_error("Pointer not found in caches");
      } else {
#ifdef DEBUG
          cout << "# delete using operator delete" << endl;
#endif
          ::operator delete(p);
      }

  }

  template<class T, size_t CACHE_SIZE>
  void otus_allocator<T, CACHE_SIZE>::construct(otus_allocator::pointer p, const_reference val) {
#ifdef DEBUG
      std::cout << "[INFO] Construct(p, val) called" << std::endl;
#endif
      new((void *) p) T(val);
  }

  template<class T, size_t CACHE_SIZE>
  template<class U, class... Args>
  void otus_allocator<T, CACHE_SIZE>::construct(U *p, Args &&... args) {
#ifdef DEBUG
      std::cout << "[INFO] Construct(p, ...args) called" << std::endl;
#endif
      ::new((void *) p) U(std::forward<Args>(args)...);
  }

  template<class T, size_t CACHE_SIZE>
  void otus_allocator<T, CACHE_SIZE>::destroy(otus_allocator::pointer p) {
#ifdef DEBUG
      std::cout << "[INFO] destroy(P* p) called" << std::endl;
#endif
      p->~T();
  }

  template<class T, size_t CACHE_SIZE>
  template<class U>
  void otus_allocator<T, CACHE_SIZE>::destroy(U *p) {
#ifdef DEBUG
      std::cout << "[INFO] destroy(U* p) called" << std::endl;
#endif
      p->~U();
  }

  template<class T, size_t CACHE_SIZE>
  typename otus_allocator<T, CACHE_SIZE>::size_type
  otus_allocator<T, CACHE_SIZE>::max_size() const {
      return std::numeric_limits<size_type>::max() / sizeof(value_type);
  }

} // namespace otus

