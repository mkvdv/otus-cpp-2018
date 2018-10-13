#include <gtest/gtest.h>

#include "../src/matrix.h"

namespace {
	using T = int;

	constexpr size_t N = 4;

	const T DEFAULT_VALUE = -1;
}

TEST(GtestSuiteMatrixN, basic) {
	otus::MatrixN<T, DEFAULT_VALUE, N> matrix; // бесконечная матрица int заполнена значениями -1
	ASSERT_EQ(matrix.size(), 0); // все ячейки свободны
	auto a = matrix[0][0][0][0];
	ASSERT_EQ(a, DEFAULT_VALUE);
	ASSERT_EQ(matrix.size(), 0);
	matrix[100][100][100][100] = 314;
	ASSERT_EQ(matrix[100][100][100][100], 314);
	ASSERT_EQ(matrix.size(), 1);

	std::stringstream ss;
	for (auto c: matrix) {
		size_t x;
		size_t y;
		size_t z;
		size_t alpha;

		int v;
		std::tie(x, y, z, alpha, v) = c;
		ss << x << y << z << alpha << v;
	}
	ASSERT_EQ("100100100100314", ss.str());
}

TEST(GtestSuiteMatrixN, access) {
	otus::MatrixN<T, DEFAULT_VALUE, N> matrix; // бесконечная матрица int заполнена значениями -1
	ASSERT_EQ(matrix.size(), 0); // все ячейки свободны

	auto a = matrix[0][0][0][0];
	ASSERT_EQ(DEFAULT_VALUE, a);
	ASSERT_EQ(matrix.size(), 0);

	a = DEFAULT_VALUE;
	ASSERT_EQ(DEFAULT_VALUE, a);
	ASSERT_EQ(matrix.size(), 0);

	matrix[100][100][100][100] = DEFAULT_VALUE;
	ASSERT_EQ(matrix.size(), 0);
	ASSERT_EQ(matrix[100][500][100][500], DEFAULT_VALUE);

	auto wr = ((matrix[100][100][100][100] = 314) = 165);
	ASSERT_EQ(matrix.size(), 1);
	ASSERT_EQ(matrix[100][100][100][100], 165);
	ASSERT_EQ(wr, 165);

	wr = 1044;
	ASSERT_EQ(wr, 1044);
	ASSERT_EQ (1044, matrix[100][100][100][100]);
	ASSERT_EQ(matrix.size(), 1);

	wr = DEFAULT_VALUE;
	ASSERT_EQ(wr, DEFAULT_VALUE);
	ASSERT_EQ (DEFAULT_VALUE, matrix[100][100][100][100]);
	ASSERT_EQ(matrix.size(), 0);

	for (int i = 0; i != 10; ++i) {
		matrix[i][i][i][i] = i;
	}
	for (int i = 0; i != 10; ++i) {
		ASSERT_EQ(matrix[i][i][i][i], i);
	}
	ASSERT_EQ(matrix.size(), 10);
}

TEST(GtestSuiteMatrixN, iteration1) {
	otus::MatrixN<T, DEFAULT_VALUE, N> matrix;

	{
		// map not used
		ASSERT_EQ(matrix.size(), 0);
		size_t counter = 0;

		for (auto x: matrix) {
			(void) x;
			++counter;
		}
		ASSERT_EQ(counter, 0);
	}

	matrix[100][100][100][100] = 314;
	auto it = matrix.begin();
	ASSERT_EQ(it, matrix.begin());
	ASSERT_EQ(matrix.end(), matrix.end());
	ASSERT_NE(it, matrix.end());
	std::tuple<size_t, size_t, size_t, size_t, T> tpl = *it;
	ASSERT_EQ(std::get<0>(tpl), 100);
	ASSERT_EQ(std::get<1>(tpl), 100);
	ASSERT_EQ(std::get<2>(tpl), 100);
	ASSERT_EQ(std::get<3>(tpl), 100);
	ASSERT_EQ(std::get<4>(tpl), 314);

	++it;
	ASSERT_EQ(it, matrix.end());

	matrix[100][100][100][100] = DEFAULT_VALUE;
	// used map
	ASSERT_EQ(matrix.size(), 0);
	size_t counter = 0;
	for (auto x: matrix) {
		(void) x;
		++counter;
	}
	ASSERT_EQ(counter, 0);
}

TEST(GtestSuiteMatrixN, iteration2) {
	otus::MatrixN<T, DEFAULT_VALUE, N> matrix;

	for (size_t i = 0; i != 10; ++i) {
		matrix[i][i + 0][i + 10][i + 20] = 1000 * i + 0;
		matrix[i][i + 1][i + 11][i + 21] = 10 * i + 1;
		matrix[i][i + 2][i + 12][i + 22] = 100000 * i + 2;
	}
	ASSERT_EQ(matrix.size(), 10 * 3);

	size_t ix0 = 0;
	size_t prev_ix0 = 0;
	size_t ix1 = 0;
	size_t prev_ix1 = 0;
	size_t ix2 = 0;
	size_t prev_ix2 = 0;
	size_t ix3 = 0;
	size_t prev_ix3 = 0;
	size_t val = 0;
	size_t prev_val = 0;

	for (std::tuple<size_t, size_t, size_t, size_t, size_t> tpl: matrix) {

		std::tie(ix0, ix1, ix2, ix3, val) = tpl;

		// use lexical_comparator here , and compare next and prev
		ASSERT_LE(std::tie(prev_ix0, prev_ix1, prev_ix2, prev_ix3, prev_val),
		          std::tie(ix0, ix1, ix2, ix3, val));

		std::tie(prev_ix0, prev_ix1, prev_ix2, prev_ix3, prev_val) = std::tie(ix0, ix1, ix2, ix3, val);
	}
}
