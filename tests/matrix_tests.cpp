#include <gtest/gtest.h>

#include "../src/matrix.h"

namespace {
	using T = int;

	const T DEFAULT_VALUE = -1;
}

TEST(GtestSuiteMatrix, basic) {
	otus::Matrix<T, DEFAULT_VALUE> matrix; // бесконечная матрица int заполнена значениями -1
	ASSERT_EQ(matrix.size(), 0); // все ячейки свободны
	auto a = matrix[0][0];
	ASSERT_EQ(a, DEFAULT_VALUE);
	ASSERT_EQ(matrix.size(), 0);
	matrix[100][100] = 314;
	ASSERT_EQ(matrix[100][100], 314);
	ASSERT_EQ(matrix.size(), 1);

	std::stringstream ss;
	for (auto c: matrix) {
		int x;
		int y;
		int v;
		std::tie(x, y, v) = c;
		ss << x << y << v;
	}
	ASSERT_EQ("100100314", ss.str());
}

TEST(GtestSuiteMatrix, access) {
	otus::Matrix<T, DEFAULT_VALUE> matrix; // бесконечная матрица int заполнена значениями -1
	ASSERT_EQ(matrix.size(), 0); // все ячейки свободны

	auto a = matrix[0][0];
	ASSERT_EQ(DEFAULT_VALUE, a);
	ASSERT_EQ(matrix.size(), 0);

	a = DEFAULT_VALUE;
	ASSERT_EQ(DEFAULT_VALUE, a);
	ASSERT_EQ(matrix.size(), 0);

	matrix[100][100] = DEFAULT_VALUE;
	ASSERT_EQ(matrix.size(), 0);
	ASSERT_EQ(matrix[100][500], DEFAULT_VALUE);

	auto wr = ((matrix[100][100] = 314) = 165);
	ASSERT_EQ(matrix.size(), 1);
	ASSERT_EQ(matrix[100][100], 165);
	ASSERT_EQ(wr, 165);

	wr = 1044;
	ASSERT_EQ(wr, 1044);
	ASSERT_EQ (1044, matrix[100][100]);
	ASSERT_EQ(matrix.size(), 1);

	wr = DEFAULT_VALUE;
	ASSERT_EQ(wr, DEFAULT_VALUE);
	ASSERT_EQ (DEFAULT_VALUE, matrix[100][100]);
	ASSERT_EQ(matrix.size(), 0);

	for (int i = 0; i != 10; ++i) {
		matrix[i][i] = i;
	}
	for (int i = 0; i != 10; ++i) {
		ASSERT_EQ(matrix[i][i], i);
	}
	ASSERT_EQ(matrix.size(), 10);
}

TEST(GtestSuiteMatrix, iteration1) {
	otus::Matrix<T, DEFAULT_VALUE> matrix;

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

	matrix[100][100] = 314;
	auto it = matrix.begin();
	ASSERT_EQ(it, matrix.begin());
	ASSERT_EQ(matrix.end(), matrix.end());
	ASSERT_NE(it, matrix.end());
	std::tuple<size_t, size_t, T> tpl = *it;
	ASSERT_EQ(std::get<0>(tpl), 100);
	ASSERT_EQ(std::get<1>(tpl), 100);
	ASSERT_EQ(std::get<2>(tpl), 314);

	++it;
	ASSERT_EQ(it, matrix.end());

	matrix[100][100] = DEFAULT_VALUE;
	// used map
	ASSERT_EQ(matrix.size(), 0);
	size_t counter = 0;
	for (auto x: matrix) {
		(void) x;
		++counter;
	}
	ASSERT_EQ(counter, 0);
}

TEST(GtestSuiteMatrix, iteration2) {
	otus::Matrix<T, DEFAULT_VALUE> matrix;

	for (size_t i = 0; i != 10; ++i) {
		matrix[i][i + 0] = 10 * i + 0;
		matrix[i][i + 1] = 10 * i + 1;
		matrix[i][i + 2] = 10 * i + 2;
	}
	ASSERT_EQ(matrix.size(), 10 * 3);

	size_t prev_ix1 = 0;
	size_t prev_ix2 = 0;

	for (std::tuple<size_t, size_t, int> tpl: matrix) {
		size_t &ix1 = std::get<0>(tpl);
		size_t &ix2 = std::get<1>(tpl);


		ASSERT_EQ(std::get<2>(tpl), 10 * ix1 + (ix2 - ix1));
		ASSERT_LE(ix1, ix2);
		ASSERT_LE(prev_ix1, ix1);
		ASSERT_TRUE(prev_ix2 <= ix2 || (prev_ix2 > ix2 && ix1 > prev_ix1));
	}
}