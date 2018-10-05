#pragma once

#include <cstddef>
#include <map>
#include <iostream>
#include <functional>

namespace otus {
	template<class T, T DEFAULT_VALUE>
	class Matrix; // foward declaration

	/**
	 * Implementaion of wrapeer, that matrix operator[] return.
	 * It do all stuff for pretty access to matrix elements.
	 */
	namespace details {
		template<class T, T DEFAULT_VALUE>
		class ElementWrapper {
		public:
			ElementWrapper(Matrix<T, DEFAULT_VALUE> &matr, size_t ix1, size_t ix2)
				: matr_(matr), first_ix_(ix1), second_ix_(ix2) {}

			ElementWrapper &operator=(T new_value) {
				matr_.assign(first_ix_, second_ix_, std::move(new_value));
				return *this;
			}

			T get() const {
				return matr_.get(first_ix_, second_ix_);
			}

		private:
			Matrix<T, DEFAULT_VALUE> &matr_;
			size_t first_ix_;
			size_t second_ix_;
		};

		template<class U, U VAL>
		bool operator==(ElementWrapper<U, VAL> const &lhs, const U &rhs) {
			return lhs.get() == rhs;
		}

		template<class U, U VAL>
		bool operator==(const U &lhs, ElementWrapper<U, VAL> const &rhs) {
			return lhs == rhs.get();
		}

		template<class U, U VAL>
		std::ostream &operator<<(std::ostream &os, ElementWrapper<U, VAL> const &rhs) {
			os << rhs.get();
			return os;
		}
	} // namespace details

	/**
	 * Class of sparse infinite 2d matrix of objects T.
	 * All cells have value DEFAULT_VALUE, if there was not any assignment to it cell.
	 * If DEFAULT_VALUE was assigned to cell, this cell not counted by real_size_ counter.
	 *
	 * Elements stored in map<KEY1, map<KEY2, VALUE>> <- maps with values inside map
	 */
	template<class T, T DEFAULT_VALUE>
	class Matrix {
	private:
		using map_t = std::map<size_t, std::map<size_t, T>>;
		using inner_map_t = std::map<size_t, T>;

	public:
		/**
		 * Helper class - intermediate state for proces of accessing element
		 * from matrix to element_wrapper (step 1 of 2. the second step is element_wrapper)
		 */
		class proxy_subscriptor {
		public:
			explicit proxy_subscriptor(Matrix<T, DEFAULT_VALUE> &matr, size_t ix)
				: matr_(matr), first_ix_(ix) {}

			details::ElementWrapper<T, DEFAULT_VALUE> operator[](size_t second_ix) {
				return details::ElementWrapper<T, DEFAULT_VALUE>(matr_, first_ix_, second_ix);
			}
			const details::ElementWrapper<T, DEFAULT_VALUE> operator[](size_t second_ix) const {
				return details::ElementWrapper<T, DEFAULT_VALUE>(matr_, first_ix_, second_ix);
			}
		private:
			Matrix<T, DEFAULT_VALUE> &matr_;
			size_t first_ix_;
		};

		proxy_subscriptor operator[](size_t idx);
		const proxy_subscriptor operator[](size_t idx) const;

		/**
		 * Have two iterators.
		 * First - to inner map in outer map
		 * Second - to element in inner map
		 */
		class matr_iterator {
		public:
			using difference_type =  size_t;
			using value_type = std::tuple<size_t, size_t, T>;

			using iterator_category= std::bidirectional_iterator_tag;

		private:
			using outer_iter_t = typename map_t::iterator;
			using inner_iter_t = typename inner_map_t::iterator;

		public:
			explicit matr_iterator(map_t &map_ref, outer_iter_t iter)
				: map_ref_(map_ref), outer_iter_(iter) {
				// set up inner_iterator, if outer_iter is valid
				if (outer_iter_ != map_ref_.end()) {
					auto &inner_map = outer_iter_->second;
					inner_iter_ = inner_map.begin();
				}
			}

			bool operator==(const matr_iterator &other) const {
				// Equality relation: iters equal <=> outer iters are equal and
				// both outer iters are end (so, the whole iterator is ended)
				// or both inner and outer iters must be eq
				return outer_iter_ == other.outer_iter_
					&& (outer_iter_ == map_ref_.end() || inner_iter_ == other.inner_iter_);
			}
			bool operator!=(const matr_iterator &other) const {
				return !(*this == other);
			}

			matr_iterator &operator++() {
				// other version
				bool incremented_inner = try_inc_inner();
				if (!incremented_inner) {
					try_inc_outer();
				}

				return *this;
			}

			value_type operator*() const {
				return std::make_tuple((*outer_iter_).first,
				                       (*inner_iter_).first,
				                       (*inner_iter_).second);
			}

		private:
			/**
			 * Increment outer iterator.
			 * Also save consistency of inner iterator.
			 */
			bool try_inc_outer() {
				if (outer_iter_ != map_ref_.end()) {
					++outer_iter_;
					if (outer_iter_ != map_ref_.end()) {
						auto &inner_map = outer_iter_->second;
						inner_iter_ = inner_map.begin();
						return true;
					}
				}
				return false;
			}

			/**
			 * Try inc inner iterator, so it refers to valid element, and @return true
			 * If can't do this (next iter is end(), or it is already end) @return false
			 */

			/**
			 *
			 * @return
			 */
			bool try_inc_inner() {
				if (outer_iter_ != map_ref_.end()) {
					auto &inner_map = outer_iter_->second;

					if (inner_iter_ != inner_map.end()) {
						++inner_iter_;
						return inner_iter_ != inner_map.end();
					}
				}
				// if fallthroug - it is not valid step
				return false;
			}


		private:
			map_t &map_ref_;
			outer_iter_t outer_iter_;
			inner_iter_t inner_iter_;
		};

		matr_iterator begin();

		matr_iterator end();

		/**
		 * Assign new value at [first_ix, second_ix] position inside maps properly
		 */
		void assign(size_t first_ix, size_t second_ix, T &&new_value);

		/**
		 * Return
		 * @param first_ix
		 * @param second_ix
		 * @return
		 */
		T get(size_t first_ix, size_t second_ix) const;

		/**
		 * Return number of real elements inside "matrix".
		 */
		size_t size() const;

	private:
		map_t mapa_{};
		size_t real_size_ = 0; // number of cells without DEFAULT_VALUE cells
	};

	/**
	 * Matrix implementation:
	 */
	template<class T, T DEFAULT_VALUE>
	size_t Matrix<T, DEFAULT_VALUE>::size() const {
		return real_size_;
	}

	template<class T, T DEFAULT_VALUE>
	typename Matrix<T, DEFAULT_VALUE>::proxy_subscriptor Matrix<T,
	                                                            DEFAULT_VALUE>::operator[](size_t idx) {
		return Matrix::proxy_subscriptor(*this, idx);
	}

	template<class T, T DEFAULT_VALUE>
	const typename Matrix<T, DEFAULT_VALUE>::proxy_subscriptor Matrix<T, DEFAULT_VALUE>::operator[](
		size_t idx) const {
		return Matrix::proxy_subscriptor(*this, idx);
	}

	template<class T, T DEFAULT_VALUE>
	typename Matrix<T, DEFAULT_VALUE>::matr_iterator Matrix<T, DEFAULT_VALUE>::begin() {
		return matr_iterator(mapa_, mapa_.begin());
	}

	template<class T, T DEFAULT_VALUE>
	typename Matrix<T, DEFAULT_VALUE>::matr_iterator Matrix<T, DEFAULT_VALUE>::end() {
		return matr_iterator(mapa_, mapa_.end());
	}

	template<class T, T DEFAULT_VALUE>
	void Matrix<T, DEFAULT_VALUE>::assign(size_t first_ix, size_t second_ix, T &&new_value) {
		auto map_iter = mapa_.find(first_ix);

		if (map_iter == mapa_.end()) { // no such key
			if (new_value != DEFAULT_VALUE) {
				mapa_[first_ix][second_ix] = std::move(new_value);
				real_size_ += 1;
			}
		} else {
			auto &inner_map = map_iter->second;
			auto value_iter = inner_map.find(second_ix);

			if (value_iter == inner_map.end()) { // no such second key
				if (new_value != DEFAULT_VALUE) {
					inner_map[second_ix] = std::move(new_value);
					real_size_ += 1;
				}
			} else {
				// both key exist
				if (new_value != DEFAULT_VALUE) {
					value_iter->second = std::move(new_value);
				} else {
					// overwrite [old value] <- default_value - now matrix not store it
					inner_map.erase(value_iter);
					real_size_ -= 1;
					if (inner_map.size() == 0) {
						// erase this map in outer map, if it became empty
						mapa_.erase(map_iter);
					}
				}
			}
		}
	}

	template<class T, T DEFAULT_VALUE>
	T Matrix<T, DEFAULT_VALUE>::get(size_t first_ix, size_t second_ix) const {
		const auto map_iter = mapa_.find(first_ix);
		if (map_iter != mapa_.end()) {
			const auto &inner_map = map_iter->second;
			const auto value_iter = inner_map.find(second_ix);
			if (value_iter != inner_map.end()) {
				return value_iter->second;
			}
		}
		return DEFAULT_VALUE;
	}

} // namespace otus


