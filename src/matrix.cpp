#include "matrix.h"

namespace {
	void matrix() {
		using T = int;
		const T DEFAULT_VALUE = 0;

		// initialize matrix
		otus::Matrix<T, DEFAULT_VALUE> matrix;
		for (int i = 0; i != 10; ++i) {
			matrix[i][i] = i;
		}
		for (size_t i = 0; i != 10; ++i) {
			matrix[i][9 - i] = 9 - i;
		}

		// print from [1,1] to [8,8];
		const size_t PRINT_BEGIN = 1;
		const size_t PRINT_END = 9;
		for (size_t i = PRINT_BEGIN; i != PRINT_END; ++i) {
			for (size_t j = PRINT_BEGIN; j != PRINT_END; ++j) {
				std::cout << matrix[i][j] << ' ';
			}
			std::cout << '\n';
		}
		std::cout << std::endl;

		// print number of real matrix cells
		std::cout << matrix.size() << '\n' << std::endl;

		// print all real cells from matrix
		for (const std::tuple<size_t, size_t, T> &tpl : matrix) {
			const size_t &ix1 = std::get<0>(tpl);
			const size_t &ix2 = std::get<1>(tpl);
			const int &val = std::get<2>(tpl);

			std::cout << "[" << ix1 << ", " << ix2 << "] = " << val << '\n';
		}
		std::cout << std::endl;
	}

} // anonymous namespace


int main() {
	matrix();

	return 0;
}
