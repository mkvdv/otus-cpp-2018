#pragma once

#include <cstddef>
#include <memory>
#include <cassert>

namespace otus {
  template<class T, size_t CAPACITY>
  class intrusive_fixed_list {
   public:
      intrusive_fixed_list();
      intrusive_fixed_list(const intrusive_fixed_list &other);
      intrusive_fixed_list(intrusive_fixed_list &&other) noexcept = default;

      intrusive_fixed_list &operator=(intrusive_fixed_list other);

      /**
       * @return true if can not allocate more elements
       */
      bool empty() const;
      size_t size() const;
      void swap(intrusive_fixed_list &lst) noexcept;

      T *alloc();
      bool contain_addr(T *ptr) const;
      void dealloc(T *ptr);

   private:
      struct Node {
          Node *next_;
          T value_;
      };

      Node *get_node(T *ptr) const;

   private:
      static constexpr size_t MIN_CAPACITY = 10;
      size_t size_ = 0;
      const std::unique_ptr<Node[]> free_list_;
      Node *head_;
  };

  template<class T, size_t CAPACITY>
  intrusive_fixed_list<T, CAPACITY>::intrusive_fixed_list()
      : free_list_(new Node[CAPACITY]), head_(free_list_.get()) {
      static_assert(CAPACITY >= MIN_CAPACITY);

      // set up intrusive list pointers
      for (size_t i = 0; i != CAPACITY - 1; ++i) {
          free_list_[i].next_ = &free_list_[i + 1];
      }
      free_list_[CAPACITY - 1].next_ = nullptr;
  }

  template<class T, size_t CAPACITY>
  intrusive_fixed_list<T, CAPACITY>::intrusive_fixed_list(const intrusive_fixed_list &other)
      : free_list_(new Node[CAPACITY]), head_(free_list_.get()) {
      (void) other; // ignore state of other allocator

      // set up intrusive list pointers
      for (size_t i = 0; i != CAPACITY - 1; ++i) {
          free_list_[i].next_ = &free_list_[i + 1];
      }
      free_list_[CAPACITY - 1].next_ = nullptr;
  }

  template<class T, size_t CAPACITY>
  void intrusive_fixed_list<T, CAPACITY>::swap(intrusive_fixed_list &lst) noexcept {
      std::swap(size_, lst.size_);
      std::swap(free_list_, lst.free_list_);
      std::swap(head_, lst.head_);
  }

  template<class T, size_t CAPACITY>
  intrusive_fixed_list<T, CAPACITY> &
  intrusive_fixed_list<T, CAPACITY>::operator=(intrusive_fixed_list other) {
      swap(other);
      return *this;
  }

  template<class T, size_t CAPACITY>
  bool intrusive_fixed_list<T, CAPACITY>::empty() const {
      return size_ == CAPACITY;
  }

  template<class T, size_t CAPACITY>
  size_t intrusive_fixed_list<T, CAPACITY>::size() const {
      return size_;
  }

  template<class T, size_t CAPACITY>
  T *intrusive_fixed_list<T, CAPACITY>::alloc() {
      assert(size_ < CAPACITY);

      Node *node = head_;
      head_ = head_->next_;
      node->next_ = nullptr;
      ++size_;

      return &node->value_;
  }

  template<class T, size_t CAPACITY>
  bool intrusive_fixed_list<T, CAPACITY>::contain_addr(T *ptr) const {
      return (char *) ptr >= (char *) free_list_.get() &&
          (char *) ptr < (char *) free_list_.get() + CAPACITY * sizeof(Node);
  }

  template<class T, size_t CAPACITY>
  void intrusive_fixed_list<T, CAPACITY>::dealloc(T *ptr) {
      assert(size_ > 0);

      Node *node = get_node(ptr);
      node->next_ = head_;
      head_ = node;
      --size_;
  }

  template<class T, size_t CAPACITY>
  typename intrusive_fixed_list<T, CAPACITY>::Node *
  intrusive_fixed_list<T, CAPACITY>::get_node(T *ptr) const {
      return (Node *) ((char *) ptr - sizeof(Node *));
  }

} // namespace otus