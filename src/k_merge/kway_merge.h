#pragma once

#include <vector>
#include <queue>

namespace otus {
  /**
   * Friendly wrapper class for merge function, wrap move_iterator to vector
   * Extract elements from vector, one at a time,
   */
  template<class T>
  class ElementExtractor {
   public:
	explicit ElementExtractor(std::vector<T> &&cnt)
		: cnt_(std::move(cnt)), next_(std::make_move_iterator(cnt_.begin())) {}

	ElementExtractor(ElementExtractor &&) noexcept = default;
	ElementExtractor &operator=(ElementExtractor &&) noexcept = default;

	bool has_next() const {
		return next_.base() != cnt_.end();
	}

	T extract() {
		T tmp = *next_;
		++next_;
		return tmp;
	}

	const T &val() const {
		return *(next_.base());
	}

   private:
	std::vector<T> cnt_;
	std::move_iterator<typename std::vector<T>::iterator> next_;
  };

  template<class T>
  bool operator<(const ElementExtractor<T> &lhs, const ElementExtractor<T> &rhs) {
	  return lhs.val() < rhs.val();
  }

  template<class T>
  bool operator>(const ElementExtractor<T> &lhs, const ElementExtractor<T> &rhs) {
	  return rhs.val() < lhs.val();
  }

  template<class T>
  bool operator==(const ElementExtractor<T> &lhs, const ElementExtractor<T> &rhs) {
	  return lhs.val() == rhs.val();
  }

  template<class T>
  bool operator!=(const ElementExtractor<T> &lhs, const ElementExtractor<T> &rhs) {
	  return !(lhs < rhs);
  }

  // Merge sorted vectors to one big sorted vector
  template<class T>
  std::vector<T> merge(std::vector<std::vector<T>> &&vec_of_seqs) {
	  std::priority_queue<ElementExtractor<T>,
						  std::vector<ElementExtractor<T>>,
						  std::greater<ElementExtractor<T>>
	  > pq;
	  size_t total_count = 0;
	  for (std::vector<T> &vec: vec_of_seqs) {
		  total_count += vec.size();
		  pq.emplace(std::move(vec));
	  }

	  std::vector<T> merged;
	  merged.reserve(total_count);

	  while (!pq.empty()) {
		  // CAUTION const_cast cause it is the only solution I've found for move value from priority_queue
		  ElementExtractor<T> elem = std::move(const_cast<ElementExtractor<T> &>(pq.top()));
		  pq.pop();
		  merged.push_back(std::move(elem.extract()));
		  if (elem.has_next()) {
			  pq.push(std::move(elem));
		  }
	  }

	  return merged;
  }

} // namespace otus
