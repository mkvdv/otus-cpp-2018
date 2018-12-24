#pragma once

#include "utils.h"

#include <array>
#include <map>
#include <vector>
#include <cassert>

namespace otus {
	template<class T, T DEFAULT_VALUE, size_t N>
	class MatrixN; // forward decl

	namespace details {
		/**
		 * Wrapper get acces to real elemnt in matrix - it store array of ixes and delegate all work
		 * to real matrix -- call it's methods get and assign
		 *
		 * todo Known bug: std::array's move and copy takes O(n), replace with std::vector // may be later
		 */
		template<class T, T DEFAULT_VALUE, size_t LAST_IX>
		class ElementWrapperN {
		public:
			static constexpr size_t N = LAST_IX + 1;

			ElementWrapperN(MatrixN<T, DEFAULT_VALUE, N> &matr, std::array<size_t, N> &&ixes)
				: matr_(matr), ixes_(std::move(ixes)) {}

		private:
			template<size_t ... Is>
			void assign_with_seq(T &&new_value, utils::seq<Is...>) {
				matr_.assign(std::move(new_value), ixes_[Is]...);
			}

			template<size_t ... Is>
			T get_with_seq(utils::seq<Is...>) const {
				return matr_.get(ixes_[Is]...);
			}

		public:

			ElementWrapperN &operator=(T new_value) {
				assign_with_seq(std::move(new_value), utils::gen_seq<N>{});
				return *this;
			}

			T get() const {
				return get_with_seq(utils::gen_seq<N>{});
			}

		private:
			MatrixN<T, DEFAULT_VALUE, N> &matr_;
			std::array<size_t, LAST_IX + 1> ixes_;
		};

		template<class U, U VAL, size_t N>
		bool operator==(const ElementWrapperN<U, VAL, N> &lhs, const U &rhs) {
			return lhs.get() == rhs;
		}

		template<class U, U VAL, size_t N>
		bool operator==(const U &lhs, const ElementWrapperN<U, VAL, N> &rhs) {
			return lhs == rhs.get();
		}

		template<class U, U VAL, size_t N>
		std::ostream &operator<<(std::ostream &os, ElementWrapperN<U, VAL, N> const &rhs) {
			os << rhs.get();
			return os;
		}
	} // namespace details

	/**
	 * Class of sparse infinite n-d matrix of objects T.
	 * All cells have value DEFAULT_VALUE, if there was not any assignment to it cell.
	 * If DEFAULT_VALUE was assigned to cell, this cell not counted by real_size_ counter.
	 *
	 * Elements stored in map<KEY_1, MatrixN<T, DEFAULT_VALUE, N-1>> <- store matrices of lower dimesion
	 * Recursion terminator is Matrix<T, DEFAULT_VALUE, 2> -- 2-d matrix,
	 * 1-d matrix is forbidden (static_assert)
 	 */
	template<class T, T DEFAULT_VALUE, size_t N>
	class MatrixN {
	public:
		using inner_t = MatrixN<T, DEFAULT_VALUE, N - 1>;
		using map_t = std::map<size_t, inner_t>;

		class matr_iterator; // forward declaration
		using iterator = matr_iterator;

		/**
		 * Helper class - intermediate state for proces of accessing element
		 * from matrix to element_wrapper (step 1 of 2. the second step is element_wrapper)
		 */
		static_assert(N > 2, "1d matrix is not a matrix, dude ¯\\_(ツ)_/¯");

		/**
		 * This class and its specialization are temp objects for "multi-layer" subscription
		 * They ix by ix save indices of subscription, forward them to next proxy object,
		 * and when it's over, thay create ElementWrapperN.
		 *
		 * operator[] can be called inly once! Else u will get exception, dude =)
		 * @tparam IX_NO index of new_index, passed to subscriptor, increments after next operator[] call
		 * @tparam Dummy dummy parameter, because we are inside class and this is C++
		 * https://stackoverflow.com/questions/2537716/why-is-partial-specialization-of-a-nested-class-template-allowed-while-complete
		 */
		template<size_t IX_NO, bool Dummy>
		class proxy_subscriptor {
		public:
			static_assert(IX_NO > 0 && IX_NO < N - 2);

			template<typename = typename std::enable_if<IX_NO != 0>::type>
			proxy_subscriptor(MatrixN<T, DEFAULT_VALUE, N> &matr,
			                  std::array<size_t, N> &&ixes,
			                  size_t next_ix)
				: matr_(matr), ixes_(std::move(ixes)) {
				ixes_[IX_NO] = next_ix;
			}

			// Was constructed with IX_NO <= N -2, so it will create other proxy proxy_subscriptor
			template<typename = typename std::enable_if<IX_NO < N - 2>::type>
			proxy_subscriptor<IX_NO + 1, Dummy> operator[](size_t next_ix) {
				if (ixes_were_moved_) {
					throw std::runtime_error("usage of proxy_subscriptor more than 1 time is invalid!");
				}
				ixes_were_moved_ = true;
				return proxy_subscriptor<IX_NO + 1, Dummy>(matr_, std::move(ixes_), next_ix);
			}

			template<typename = typename std::enable_if<IX_NO < N - 2>::type>
			const proxy_subscriptor<IX_NO + 1, Dummy> operator[](size_t next_ix) const {
				if (ixes_were_moved_) {
					throw std::runtime_error("usage of proxy_subscriptor more than 1 time is invalid!");
				}
				ixes_were_moved_ = true;
				return proxy_subscriptor<IX_NO + 1, Dummy>(matr_, std::move(ixes_), next_ix);
			}

		private:
			MatrixN<T, DEFAULT_VALUE, N> &matr_;
			std::array<size_t, N> ixes_;
			bool ixes_were_moved_ = false;
		}; // class proxy_subscriptor

		/**
		 * Partial specialization for IX_NO == 0
		 */
		template<bool Dummy>
		class proxy_subscriptor<0, Dummy> {
		public:
			proxy_subscriptor(MatrixN<T, DEFAULT_VALUE, N> &matr, size_t first_ix)
				: matr_(matr) {
				ixes_[0] = first_ix;
			}

			proxy_subscriptor<1, Dummy> operator[](size_t next_ix) {
				if (ixes_were_moved_) {
					throw std::runtime_error("usage of proxy_subscriptor more than 1 time is invalid!");
				}
				ixes_were_moved_ = true;
				return proxy_subscriptor<1, Dummy>(matr_, std::move(ixes_), next_ix);

			}

			const proxy_subscriptor<1, Dummy> operator[](size_t next_ix) const {
				if (ixes_were_moved_) {
					throw std::runtime_error("usage of proxy_subscriptor more than 1 time is invalid!");
				}
				ixes_were_moved_ = true;
				return proxy_subscriptor<1, Dummy>(matr_, std::move(ixes_), next_ix);
			}

		private:
			MatrixN<T, DEFAULT_VALUE, N> &matr_;
			std::array<size_t, N> ixes_;
			bool ixes_were_moved_ = false;
		}; // class proxy_subscriptor<0, Dummy>

		template<bool Dummy>
		class proxy_subscriptor<N - 2, Dummy> {
		public:
			static const size_t IX_NO = N - 2;

			proxy_subscriptor(MatrixN<T, DEFAULT_VALUE, N> &matr,
			                  std::array<size_t, N> &&ixes,
			                  size_t next_ix)
				: matr_(matr), ixes_(std::move(ixes)) {
				ixes_[IX_NO] = next_ix;
			}

			// Was constructed with IX_NO == N - 2, so next operator[] is access to reference to
			// value
			template<typename = typename std::enable_if<IX_NO == N - 2>::type>
			details::ElementWrapperN<T, DEFAULT_VALUE, IX_NO + 1> operator[](size_t next_ix) {
				if (ixes_were_moved_) {
					throw std::runtime_error("usage of proxy_subscriptor more than 1 time is invalid!");
				}
				ixes_[N - 1] = next_ix; // set it manually here, look at ElementWrapper ctor
				ixes_were_moved_ = true;
				return details::ElementWrapperN<T, DEFAULT_VALUE, IX_NO + 1>(matr_, std::move(ixes_));
			}

			const details::ElementWrapperN<T, DEFAULT_VALUE, IX_NO + 1> operator[](size_t next_ix) const {
				if (ixes_were_moved_) {
					throw std::runtime_error("usage of proxy_subscriptor more than 1 time is invalid!");

				}
				ixes_[N - 1] = next_ix;
				ixes_were_moved_ = true;
				return details::ElementWrapperN<T, DEFAULT_VALUE, IX_NO + 1>(matr_, std::move(ixes_));
			}

		private:
			MatrixN<T, DEFAULT_VALUE, N> &matr_;
			std::array<size_t, N> ixes_;
			bool ixes_were_moved_ = false;
		}; // class proxy_subscriptor<N - 2, Dummy>

		/**
		 * Work differently, depends on IX_NO param. Implemented using SFINAE
		 * Accumulate ixes for matrix subscribtion
		 * @tparam IX_NO current index,  0 <= IX_NO < N
		 */
		static constexpr bool DUMMY = false;
		proxy_subscriptor<0, DUMMY> operator[](size_t idx) {
			return proxy_subscriptor<0, DUMMY>(*this, idx);
		}
		const proxy_subscriptor<0, DUMMY> operator[](size_t idx) const {
			return proxy_subscriptor<0, DUMMY>(*this, idx);
		}

		/**
		 * This iterator hold first_iterator - iterator to map of matrices
		 * and second_iterator - inner iterator, iterate over inner_matrix
		 *
		 * It recursively call incerements of onner oterators, and so they increments properly
		 */
		class matr_iterator {
		public:
			using difference_type =  size_t;
			using iterator_category= std::forward_iterator_tag;

		private:
			using map_iter_t = typename map_t::iterator;
			using inner_matrix_iter_t = typename MatrixN<T, DEFAULT_VALUE, N - 1>::iterator;

		public:
			matr_iterator() = default; // Cause inner_matrix_iter_ must have and, and so this class do

			matr_iterator(map_t *map_ptr, map_iter_t map_iter)
				: map_ptr_(map_ptr), map_iter_(map_iter) {
				// set up inner_iterator, if outer_iter is valid
				if (map_iter_ != map_ptr_->end()) {
					auto &inner_map = map_iter_->second;
					inner_matrix_iter_ = inner_map.begin();
				}
			}

			matr_iterator &operator=(matr_iterator other) {
				std::swap(map_ptr_, other.map_ptr_);
				std::swap(map_iter_, other.map_iter_);
				std::swap(inner_matrix_iter_, other.inner_matrix_iter_);

				return *this;
			}

			bool operator==(const matr_iterator &other) const {
				// Equality relation: iters equal <=> outer iters are equal and
				// both outer iters are end (so, the whole iterator is ended)
				// or both inner and outer iters must be eq
				return map_iter_ == other.map_iter_
					&& (map_iter_ == map_ptr_->end() || inner_matrix_iter_ == other.inner_matrix_iter_);
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

			/**
			 * Call recursively to the lowest iterator, and he will get real value.
			 * @return
			 */
			T get_current_value() const {
				return inner_matrix_iter_.get_current_value();
			}

		private:
			/**
			 * Need for generate properly list of ixes like argument list. Use index sequence idioma.
			 * God, save the C++ templates.
			 */
			template<size_t ...Is>
			auto tuple_packer(const std::vector<size_t> &ixes, utils::seq<Is...>) const {
				return std::make_tuple(ixes[Is]..., get_current_value());
			}

		public:
			/**
			 * Take KEY1 from current map and push it in ixes_result vector.
			 * and than recursively do it - so ixes_result will store "coordinates" of current element,
			 * in which this iterator (chain of iterators) points to.
			 *
			 * @param [in, out] ixes_result
			 * @param this_ix indexes of this index in vector, increments in recursion call
			 */
			void fill_ixes_vector(std::vector<size_t> &ixes_result, size_t this_ix) const {
				assert(this_ix < ixes_result.size());

				ixes_result[this_ix] = (*map_iter_).first;
				inner_matrix_iter_.fill_ixes_vector(ixes_result, this_ix + 1);
			}

			auto operator*() const {
				std::vector<size_t> ixes(N, 0);

				fill_ixes_vector(ixes, 0);
				return tuple_packer(ixes, utils::gen_seq<N>{});
			}

			// todo make working type alias for value_type
			// using value_type = decltype(std::declval<matr_iterator>().operator*());
			// else  how to generate type std::tuple<size_t, size_t, ..., size_t, T>

		private:
			/**
			 * Increment outer iterator.
			 * Also save consistency of inner iterator (recursive chain of iterators)
			 */
			bool try_inc_outer() {
				if (map_iter_ != map_ptr_->end()) {
					++map_iter_;
					if (map_iter_ != map_ptr_->end()) {
						auto &inner_map = map_iter_->second;
						inner_matrix_iter_ = inner_map.begin();
						return true;
					}
				}
				return false;
			}

			/**
			 * Try inc inner iterator, so it refers to valid element, and @return true
			 * If can't do this (next iter is end(), or it is already end) @return false
			 */
			bool try_inc_inner() {
				if (map_iter_ != map_ptr_->end()) {
					auto &inner_map = map_iter_->second;

					if (inner_matrix_iter_ != inner_map.end()) {
						++inner_matrix_iter_;
						return inner_matrix_iter_ != inner_map.end();
					}
				}
				// if fallthroug - it is not valid step
				return false;
			}

		private:
			const map_t *map_ptr_ = nullptr; // cause matr_iterator() must be
			map_iter_t map_iter_{};
			inner_matrix_iter_t inner_matrix_iter_{};
		};

		matr_iterator begin() {
			return matr_iterator(&mapa_, mapa_.begin());
		}

		matr_iterator end() {
			return matr_iterator(&mapa_, mapa_.end());
		}

		/**
		 * Assign new value at [first_ix, second_ix] position inside maps properly
		 */

		template<class... Args>
		void assign(T &&new_value, size_t first_ix, Args... ixes) {
			auto map_iter = mapa_.find(first_ix);

			if (map_iter == mapa_.end()) { // no such key @first_ix
				if (new_value != DEFAULT_VALUE) {
					// create new matrix with this element in push it inside
					inner_t inner_matrix;
					inner_matrix.assign(std::move(new_value), ixes...);
					mapa_[first_ix] = std::move(inner_matrix);
					real_size_ += 1;
				}
			} else { // hav key @first_ix
				inner_t &inner_matr = map_iter->second;
				size_t prev_size = inner_matr.size();

				inner_matr.assign(std::move(new_value), ixes...);
				if (inner_matr.size() != prev_size) {
					size_t new_size = inner_matr.size();
					real_size_ = real_size_ - prev_size + new_size;

					if (!inner_matr.size()) {
						mapa_.erase(map_iter); // else during iteration there will be redundant iters
					}
				}
			}
		}

		/**
		 * Return
		 * @param first_ix
		 * @param second_ix
		 * @return
		 */
		template<class... Args>
		T get(size_t first_ix, Args... ixes) const {
			const auto map_iter = mapa_.find(first_ix);
			if (map_iter != mapa_.end()) {
				const inner_t &inner_matr = map_iter->second;
				return inner_matr.get(ixes...);
			}
			return DEFAULT_VALUE;
		}

		/**
		 * Return number of real elements inside "matrix".
		 */
		size_t size() const {
			return real_size_;
		}

	private:
		map_t mapa_{};
		size_t real_size_ = 0; // number of cells without DEFAULT_VALUE cells

	}; // class MatrixN


	/**
	 * Recursion terminator of non-specialized template class.
	 * If u remove N parameter, this will be simple 2-d spaerse infinite matrix.
	 */
	template<class T, T DEFAULT_VALUE>
	class MatrixN<T, DEFAULT_VALUE, 2> {
	public:
		using inner_t = std::map<size_t, T>;
		using map_t = std::map<size_t, inner_t>;

		class matr_iterator; // forward declaration
		using iterator = matr_iterator;

	public:
		/**
		 * Helper class - intermediate state for proces of accessing element
		 * from matrix to element_wrapper (step 1 of 2. the second step is element_wrapper)
		 */
		class proxy_subscriptor {
		public:
			explicit proxy_subscriptor(MatrixN<T, DEFAULT_VALUE, 2> &matr, size_t ix)
				: matr_(matr), first_ix_(ix) {}

			static constexpr size_t LAST_IX = 1;

			details::ElementWrapperN<T, DEFAULT_VALUE, LAST_IX> operator[](size_t second_ix) {
				std::array<size_t, 2> ixes = {first_ix_, second_ix};
				return details::ElementWrapperN<T, DEFAULT_VALUE, LAST_IX>(matr_, std::move(ixes));
			}

			const details::ElementWrapperN<T, DEFAULT_VALUE, LAST_IX> operator[](size_t second_ix) const {
				std::array<size_t, 2> ixes = {first_ix_, second_ix};
				return details::ElementWrapperN<T, DEFAULT_VALUE, LAST_IX>(matr_, std::move(ixes));
			}

		private:
			MatrixN<T, DEFAULT_VALUE, 2> &matr_;
			size_t first_ix_;
		};

		proxy_subscriptor operator[](size_t idx) {
			return proxy_subscriptor(*this, idx);
		}
		const proxy_subscriptor operator[](size_t idx) const {
			return proxy_subscriptor(*this, idx);
		}

		/**
		 * Have two iterators.
		 * First - to inner map in outer map
		 * Second - to element in inner map
		 */
		class matr_iterator {
		public:
			using difference_type =  size_t;
			using value_type = std::tuple<size_t, size_t, T>;
			using iterator_category= std::forward_iterator_tag;

		private:
			using map_iter_t = typename map_t::iterator;
			using inner_map_iter_t = typename inner_t::iterator;

		public:
			matr_iterator() {}

			matr_iterator(map_t *map_ptr, map_iter_t iter)
				: map_ptr_(map_ptr), map_iter_(iter) {
				// set up inner_iterator, if outer_iter is valid
				if (map_iter_ != map_ptr_->end()) {
					auto &inner_map = map_iter_->second;
					inner_map_iter_ = inner_map.begin();
				}
			}

			bool operator==(const matr_iterator &other) const {
				// Equality relation: iters equal <=> outer iters are equal and
				// both outer iters are end (so, the whole iterator is ended)
				// or both inner and outer iters must be eq
				return map_iter_ == other.map_iter_
					&& (map_iter_ == map_ptr_->end() || inner_map_iter_ == other.inner_map_iter_);
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

			void fill_ixes_vector(std::vector<size_t> &ixes_result, size_t this_ix) const {
				assert(this_ix == ixes_result.size() - 2);
				ixes_result[this_ix] = (*map_iter_).first;

				ixes_result[this_ix + 1] = (*inner_map_iter_).first;
			}

			T get_current_value() const {
				return (*inner_map_iter_).second;
			}

			value_type operator*() const {
				return std::make_tuple((*map_iter_).first,
				                       (*inner_map_iter_).first,
				                       (*inner_map_iter_).second);
			}

		private:
			/**
			 * Increment outer iterator.
			 * Also save consistency of inner iterator.
			 */
			bool try_inc_outer() {
				if (map_iter_ != map_ptr_->end()) {
					++map_iter_;
					if (map_iter_ != map_ptr_->end()) {
						auto &inner_map = map_iter_->second;
						inner_map_iter_ = inner_map.begin();
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
				if (map_iter_ != map_ptr_->end()) {
					auto &inner_map = map_iter_->second;

					if (inner_map_iter_ != inner_map.end()) {
						++inner_map_iter_;
						return inner_map_iter_ != inner_map.end();
					}
				}
				// if fallthrough - it is not valid step
				return false;
			}

		private:
			map_t *map_ptr_ = nullptr;
			map_iter_t map_iter_{};
			inner_map_iter_t inner_map_iter_{};
		};

		matr_iterator begin() {
			return matr_iterator(&mapa_, mapa_.begin());
		}

		matr_iterator end() {
			return matr_iterator(&mapa_, mapa_.end());
		}

		/**
		 * Assign new value at [first_ix, second_ix] position inside maps properly
		 */
		void assign(T &&new_value, size_t first_ix, size_t second_ix) {
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
							mapa_.erase(map_iter);  // else during iteration there will be redundant iters
						}
					}
				}
			}
		}

		/**
		 * Return
		 * @param first_ix
		 * @param second_ix
		 * @return
		 */
		T get(size_t first_ix, size_t second_ix) const {
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

		/**
		 * Return number of real elements inside "matrix".
		 */
		size_t size() const {
			return real_size_;
		}

	private:
		map_t mapa_{};
		size_t real_size_ = 0; // number of cells without DEFAULT_VALUE cells
	};

	// frienly alias, so old test code for 2-d matrix will also work =)
	template<class T, T DEFAULT_VALUE> using Matrix = MatrixN<T, DEFAULT_VALUE, 2>;

} // namespace otus
