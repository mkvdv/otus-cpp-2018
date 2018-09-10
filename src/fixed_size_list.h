#pragma once

#include <cstddef>
#include <memory>
#include <cassert>
#include <functional>
#include "utils.h"

namespace otus {
  template<class T, size_t CAPACITY>
  class fixed_size_list {
   public:
      fixed_size_list();

      // nocopyable
      fixed_size_list(const fixed_size_list &) = delete;
      fixed_size_list &operator=(const fixed_size_list &) = delete;

      fixed_size_list(fixed_size_list &&other) noexcept;
      fixed_size_list &operator=(fixed_size_list &&other) noexcept;

      ~fixed_size_list();

      /**
       * @return true if can not allocate more elements
       */
      bool filled() const;
      size_t size() const;
      void swap(fixed_size_list &lst) noexcept;

      T *alloc(); // Don't initialize this memory, it will be initialized in allocator
      bool contain_addr(T *ptr) const;
      void dealloc(T *ptr); // don't destroy this

//   private:
      struct Node {
          Node *next_;
          T value_;
      };

      Node *get_node(T *ptr) const;

   private:
      static constexpr size_t MIN_CAPACITY = 10;
      size_t size_ = 0;
//      std::unique_ptr<Node[], std::function<void(Node *)>> free_list_ = nullptr;
      Node *free_list_ = nullptr;
      Node *head_ = nullptr;
  };

  template<class T, size_t CAPACITY>
  fixed_size_list<T, CAPACITY>::fixed_size_list()
      : free_list_(static_cast<Node *>(::operator new[](CAPACITY * sizeof(Node)))),
        head_(free_list_) {
      static_assert(CAPACITY >= MIN_CAPACITY);
      assert(free_list_);

      // set up intrusive list pointers
      for (size_t i = 0; i != CAPACITY - 1; ++i) {
          free_list_[i].next_ = &free_list_[i + 1];
      }
      free_list_[CAPACITY - 1].next_ = nullptr;
  }

  template<class T, size_t CAPACITY>
  fixed_size_list<T, CAPACITY>::~fixed_size_list() {
      ::operator delete[](free_list_);
  }

  /*
   * THIS IS BAD TODO REMOVE
//  template<class T, size_t CAPACITY>
//  fixed_size_list<T, CAPACITY>::fixed_size_list(const fixed_size_list &other)
//      : free_list_(new Node[CAPACITY]), head_(free_list_.get()) {
//      (void) other; // ignore state of other allocator
//
//      // set up intrusive list pointers
//      for (size_t i = 0; i != CAPACITY - 1; ++i) {
//          free_list_[i].next_ = &free_list_[i + 1];
//      }
//      free_list_[CAPACITY - 1].next_ = nullptr;
//  }
*/

  template<class T, size_t CAPACITY>
  void fixed_size_list<T, CAPACITY>::swap(fixed_size_list &lst) noexcept {
      std::swap(size_, lst.size_);
      std::swap(free_list_, lst.free_list_);
      std::swap(head_, lst.head_);
  }

  template<class T, size_t CAPACITY>
  fixed_size_list<T, CAPACITY>::fixed_size_list(fixed_size_list &&other) noexcept {
      swap(other);
  }

  template<class T, size_t CAPACITY>
  fixed_size_list<T, CAPACITY> &
  fixed_size_list<T, CAPACITY>::operator=(fixed_size_list &&other) noexcept {
      swap(other);
      return *this;
  }

  template<class T, size_t CAPACITY>
  bool fixed_size_list<T, CAPACITY>::filled() const {
      return size_ == CAPACITY;
  }

  template<class T, size_t CAPACITY>
  size_t fixed_size_list<T, CAPACITY>::size() const {
      return size_;
  }

  template<class T, size_t CAPACITY>
  T *fixed_size_list<T, CAPACITY>::alloc() {
      assert(size_ < CAPACITY);

      Node *node = head_;
      head_ = head_->next_;
      node->next_ = nullptr;
      ++size_;

      return &node->value_;
  }

  template<class T, size_t CAPACITY>
  bool fixed_size_list<T, CAPACITY>::contain_addr(T *ptr) const {
      return (char *) ptr >= (char *) free_list_ &&
          (char *) ptr < (char *) free_list_ + CAPACITY * sizeof(Node);
  }

  template<class T, size_t CAPACITY>
  void fixed_size_list<T, CAPACITY>::dealloc(T *ptr) {
      assert(size_ > 0);

      Node *node = get_node(ptr);
      node->next_ = head_;
      head_ = node;
      --size_;
  }

  template<class T, size_t CAPACITY>
  typename fixed_size_list<T, CAPACITY>::Node *
  fixed_size_list<T, CAPACITY>::get_node(T *ptr) const {
//      using node_t = fixed_size_list<T, CAPACITY>::Node;

      // #1: standard offsetof - bad
//      return (Node *) ((char *) ptr - offsetof(node_t, value_)); // not work with std::map

      //  #2: custom offset_of - need default ctor
//      return (Node *) ((char *) ptr - utils::offset_of(&node_t::value_));

      // #3: hardcode - suck, but work
      return (Node *) ((char *) ptr - sizeof(Node *)); // todo fix me, please
  }

} // namespace otus