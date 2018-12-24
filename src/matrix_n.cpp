#include "matrix.h"

#include <cassert>
#include <iostream>
#include <sstream>

namespace {
	void matrix_n() {
		using T = char;
		const T DEFAULT_VALUE = '0';
		const size_t N = 4;

		// initialize matrix
		otus::MatrixN<T, DEFAULT_VALUE, N> matrix_n;
		for (size_t i = 0; i != N; ++i) {
			matrix_n[i][i][i][i] = static_cast<T>('1' + i);
			assert(matrix_n[i][i][i][i] == static_cast<T>('1' + i));
		}

		assert(matrix_n.size() == N);

		// print
		for (size_t i = 0; i != N; ++i) {
			for (size_t j = 0; j != N; ++j) {
				std::cout << "[i, j] = [" << i << ", " << j << "]" << '\n';
				for (size_t k = 0; k != N; ++k) {
					for (size_t l = 0; l != N; ++l) {
						std::cout << matrix_n[i][j][k][l] << ' ';
					}
					std::cout << '\n';
				}
				std::cout << '\n';
			}
			std::cout << '\n';
		}
		std::cout << std::endl;

		/**
		 * Print all real elements, using iterator
		 */

		std::stringstream ss;
		for (std::tuple<size_t, size_t, size_t, size_t, T> tpl: matrix_n) {
			size_t x;
			size_t y;
			size_t z;
			size_t alpha;
			T c;

			std::tie(x, y, z, alpha, c) = tpl;
			ss << c;
		}
		assert(ss.str() == "1234");

	}

} // anonymous namespace


int main() {
	matrix_n();

	return 0;
}



