#pragma once

#include <memory>

namespace otus {

  namespace details {
    template<class T>
    struct mylist_node {
        mylist_node *next;
        T value;
    };
  }

  /**
   * Linked list, can only add to head (to front), can't remove =)
   */
  template<
      class T,
      class Alloc = std::allocator<details::mylist_node<T>>
  >
  class mylist {
   public:
      using node_t = details::mylist_node<T>;
      using NodeAllocator_t = typename std::allocator_traits<Alloc>::template rebind_alloc<node_t>;

      using value_type = T;
      using allocator_type = Alloc; // but really we use NodeAllocator
      using size_type = std::size_t;
      using difference_type = std::ptrdiff_t;
      using reference = value_type &;
      using const_reference = const value_type &;
      using pointer = T *;
      using const_pointer = const T *;

      class mylist_iterator; // forward decl
      using iterator = mylist_iterator;
      using const_iterator = const mylist_iterator;

      class mylist_iterator : std::iterator<std::forward_iterator_tag, T> {
       public:
          explicit mylist_iterator(node_t *ptr = nullptr)
              : ptr_(ptr) {}

          mylist_iterator &operator++() {
              if (ptr_) {
                  ptr_ = ptr_->next;
              } else {
                  throw std::runtime_error("Invalid iterator");
              }
              return *this;
          }
          mylist_iterator operator++(int) {
              mylist_iterator retval = *this;
              ++(*this);
              return retval;
          }
          bool operator==(mylist_iterator other) const {
              return ptr_ == other.ptr_;
          }
          bool operator!=(mylist_iterator other) const {
              return !(*this == other);
          }
          typename std::iterator<std::forward_iterator_tag, T>::reference
          operator*() const { return ptr_->value; }

       private:
          node_t *ptr_;
      };

      mylist() = default;
      mylist(const mylist &); //  = delete;
      ~mylist();

      mylist(mylist &&) noexcept;
      // move and copy at the same time, swap trick
      mylist &operator=(mylist_iterator other);

      // todo test it
      void swap(mylist &lst) noexcept;

      void push_front(const T &val);

      void push_front(T &&val);

      template<class... Args>
      void emplace_front(Args &&... args);

      mylist_iterator begin();

      mylist_iterator end();

      size_t size() const;

   private:
      node_t *head_ = nullptr;
      NodeAllocator_t allocator{};
      size_t size_ = 0;
  };

  /**
   * mylist_impl.h
   */

  template<class T, class Alloc>
  mylist<T, Alloc>::~mylist() {
      while (head_) {
          node_t *next = head_->next;
          allocator.destroy(&head_->value);
          allocator.deallocate(head_, 1);
          head_ = next;
      }
  }

  template<class T, class Alloc>
  mylist<T, Alloc>::mylist(const mylist &other)
      : size_(other.size_) {
      // Section 1, point 6 - "Build — Temporary Dummy List Node" of
      // http://cslibrary.stanford.edu/105/LinkedListProblems.pdf
      node_t *const dummy = allocator.allocate(1);
      node_t *tail = dummy;
      dummy->next = nullptr;

      node_t *cur_other = other.head_;

      while (cur_other) {
          node_t *new_node = allocator.allocate(1);
          allocator.construct(new_node, {nullptr, cur_other->value});

          tail->next = new_node;
          tail = new_node;

          cur_other = cur_other->next;
      }
      head_ = dummy->next;
      allocator.deallocate(dummy, 1);
  }

  template<class T, class Alloc>
  mylist<T, Alloc>::mylist(mylist &&other) noexcept {
      swap(other);
  }

  template<class T, class Alloc>
  mylist<T, Alloc> &mylist<T, Alloc>::operator=(mylist::mylist_iterator other) {
      swap(other);
      return *this;
  }

  template<class T, class Alloc>
  void mylist<T, Alloc>::swap(mylist &lst) noexcept {
      std::swap(head_, lst.head_);
      std::swap(allocator, lst.allocator);
      std::swap(size_, lst.size_);
  }

  template<class T, class Alloc>
  void mylist<T, Alloc>::push_front(const T &val) {
      node_t *new_node = allocator.allocate(1);
      allocator.construct(new_node, node_t{nullptr, val});
      new_node->next = head_;
      head_ = new_node;
      ++size_;
  }

  template<class T, class Alloc>
  void mylist<T, Alloc>::push_front(T &&val) {
      node_t *new_node = allocator.allocate(1);
      allocator.construct(new_node, node_t{nullptr, std::move(val)});
      new_node->next = head_;
      head_ = new_node;
      ++size_;
  }

  template<class T, class Alloc>
  template<class... Args>
  void mylist<T, Alloc>::emplace_front(Args &&... args) {
      node_t *new_node = allocator.allocate(1);
      allocator.construct(new_node, node_t{nullptr, T(std::forward<Args>(args)...)});
      new_node->next = head_;
      head_ = new_node;
      ++size_;
  }

  template<class T, class Alloc>
  typename mylist<T, Alloc>::mylist_iterator mylist<T, Alloc>::begin() {
      return mylist_iterator(head_);
  }

  template<class T, class Alloc>
  typename mylist<T, Alloc>::mylist_iterator
  mylist<T, Alloc>::end() {
      return mylist_iterator();
  }

  template<class T, class Alloc>
  size_t mylist<T, Alloc>::size() const {
      return size_;
  }

} // namespace otus
