#pragma once

#include "k_merge/kway_merge.h"

#include <boost/thread/thread.hpp>
#include <boost/log/trivial.hpp>

#include <vector>
#include <string>
#include <thread>
#include <fstream>
#include <future>
#include <functional>

namespace otus {
  /**
   * Types of functions:
   *  - map_function			:: std::vector<T1> -> std::vector<T2>
   *  - reduce_function			:: std::vector<T2> -> T3
   *  - final_reduce_function	:: std::vector<T3> -> T4
   */
  template<class T1, class T2, class T3, class T4>
  class MapReducer {
   public:
	using vec_of_lines =std::vector<std::string>;

	MapReducer(size_t map_threads, size_t reduce_threads_)
		: map_threads_(map_threads), reduce_threads_(reduce_threads_) {}

	T4 run(std::vector<T1> &&input,
		   std::function<std::vector<T2>(std::vector<T1> &&)> map_function,
		   std::function<T3(std::vector<T2> &&)> reduce_function,
		   std::function<T4(std::vector<T3> &&)> final_reduce_function) {

		// split lines to map_threads_ bunches
		std::vector<std::vector<T1>> splitted = split(std::move(input), map_threads_);

		// run MAP :: std::vector<T1> -> std::vector<T2> step
		std::vector<std::vector<T2>> results_of_mapping = map_step(map_function, std::move(splitted));
		BOOST_LOG_TRIVIAL(info) << "mapped";

		// sort all vectors results
		for (std::vector<T2> &vec : results_of_mapping) {
			std::sort(vec.begin(), vec.end());
		}
		BOOST_LOG_TRIVIAL(info) << "sorted";

		// MERGE them all
		std::vector<T2> sorted_merged = otus::merge<T2>(std::move(results_of_mapping));
		BOOST_LOG_TRIVIAL(info) << "MERGED:";
		for (auto &val: sorted_merged) {
			BOOST_LOG_TRIVIAL(info) << "\'" << val << "\'";
		}

		std::vector<std::vector<T2>> splitted_mapped = split_for_reducing(std::move(sorted_merged),
																		  reduce_threads_);
		BOOST_LOG_TRIVIAL(info) << "splitted in " << splitted_mapped.size() << " bunches";
		for (auto &vec: splitted_mapped) {
			BOOST_LOG_TRIVIAL(info) << "# vec";
			for (T2 &elem: vec) {
				BOOST_LOG_TRIVIAL(info) << "\'" << elem << "\'";
			}
		}

		// do REDUCE in many threads :: std::vector<T2> -> T3
		std::vector<T3> results_of_reduce = reduce_step(reduce_function, std::move(splitted_mapped));
		BOOST_LOG_TRIVIAL(info) << "reduced bunches";
		for (T3 &one_of_res : results_of_reduce) {
			BOOST_LOG_TRIVIAL(info) << "One of reduced: " << one_of_res;
		}

		//make final REDUCE :: std::vector<T3> -> T4, and we can extract answer from it
		T4 reduce_result = final_reduce_function(std::move(results_of_reduce));
		BOOST_LOG_TRIVIAL(info) << "reduced to one result: " << reduce_result;

		return reduce_result;
	}

   private:
	std::vector<std::vector<T2>> map_step(std::function<std::vector<T2>(std::vector<T1> &&)> map_function,
										  std::vector<std::vector<T1>> &&splitted) {
		std::vector<std::vector<T2>> results_of_mapping;

		// prepare returning value from threads
		std::vector<std::promise<std::vector<T2>>> promise_of_mapping(splitted.size());
		std::vector<std::future<std::vector<T2>>> future_of_mapping;
		for (size_t i = 0; i != promise_of_mapping.size(); ++i) {
			future_of_mapping.push_back(promise_of_mapping[i].get_future());
		}

		// start threads with worker function
		auto map_worker = [map_function](std::promise<std::vector<T2>> &&promise, std::vector<T1> &&arg) {
			std::vector<T2> res = map_function(std::move(arg));
			promise.set_value(std::move(res));
			// todo handle exception
		};
		std::vector<std::thread> mapping_workers;
		for (size_t i = 0; i != promise_of_mapping.size(); ++i) {
			mapping_workers.emplace_back(map_worker,
										 std::move(promise_of_mapping[i]),
										 std::move(splitted[i]));
		}
		for (auto &t: mapping_workers) {
			if (t.joinable()) {
				t.join();
			}
		}

		for (size_t i = 0; i != future_of_mapping.size(); ++i) {
			results_of_mapping.push_back(future_of_mapping[i].get());
		}
		return results_of_mapping;
	}

	std::vector<T3> reduce_step(std::function<T3(std::vector<T2> &&)> &reduce_function,
								std::vector<std::vector<T2>> &&input) {
		std::vector<T3> results_of_reduce;

		std::vector<std::promise<T3>> promise_of_reducing(input.size());
		std::vector<std::future<T3>> future_of_reducing;
		for (size_t i = 0; i != input.size(); ++i) {
			future_of_reducing.push_back(promise_of_reducing[i].get_future());
		}

		// start threads with worker function
		auto reduce_worker = [reduce_function](std::promise<T3> &&promise,
											   std::vector<T2> &&arg) {
			promise.set_value(std::move(reduce_function(std::move(arg))));
			// todo handle exception
		};
		std::vector<std::thread> reduce_workers;
		for (size_t i = 0; i != input.size(); ++i) {
			reduce_workers.emplace_back(reduce_worker,
										std::move(promise_of_reducing[i]),
										std::move(input[i]));
		}
		for (auto &t: reduce_workers) {
			if (t.joinable()) {
				t.join();
			}
		}

		for (size_t i = 0; i != input.size(); ++i) {
			results_of_reduce.push_back(future_of_reducing[i].get());
		}

		return results_of_reduce;
	}

	/**
	 * @param long_vec is a vector of sorted T2 elements
	 * @return vector of vectors, all uniq elements contain this shit
	 */
	std::vector<std::vector<T2>> split_for_reducing(std::vector<T2> &&long_vec, size_t parts) {
		// split long_vec to reduce_threads sorted vectors, contain all vectors
		std::vector<std::vector<T2>> res;

		size_t per_bunch_roundup = (long_vec.size() / parts)
			+ (long_vec.size() % parts ? 1 : 0);
		size_t total_transferred = 0;

		// push equal elements at the same bunch
		auto it = long_vec.begin();
		auto it_prev = it;

		while (it_prev != long_vec.end()) {
			// create new bunch
			res.emplace_back();
			std::vector<T2> &bunch = res.back();

			// fill this bunch
			size_t inserted_count = 0;
			while (inserted_count < per_bunch_roundup) {
				if (it == long_vec.end() || *it != *it_prev) {
					std::copy(std::make_move_iterator(it_prev),
							  std::make_move_iterator(it),
							  std::back_inserter(bunch));
					long moved_count = std::distance(it_prev, it);
					inserted_count += moved_count;
					total_transferred += moved_count;

					it_prev = it;
					if (it_prev == long_vec.end()) {
						break; // all done
					}
					++it;
				} else {
					++it;
				}
			}
			if (it_prev == long_vec.end()) {
				break; // all done
			}
		}

		assert(total_transferred == long_vec.size());

		return res;
	}

	std::vector<std::vector<T1>> split(std::vector<T1> &&lines, size_t parts) {
		std::vector<std::vector<T1>> splitted;

		size_t splitted_ix = 0;
		for (size_t i = 0; i != lines.size(); ++i) {
			if (splitted.size() < splitted_ix + 1) {
				splitted.push_back(std::vector<T1>{});
			}
			splitted[splitted_ix].push_back(std::move(lines[i]));
			splitted_ix = (splitted_ix + 1) % parts;
		}

		return splitted;
	}

   private:
	const size_t map_threads_;
	const size_t reduce_threads_;
  };

} // namespace otus
