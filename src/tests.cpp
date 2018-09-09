#include "utils.h"
#include "filters.h"
#include <limits>

#include <gtest/gtest.h>


TEST(GtestSuiteMain, split)
{
	using namespace otus;
	std::string str = "192.168.0.1";
	ip_t result = split<uint8_t>(str, '.', [](const std::string & s) {
		return static_cast<uint8_t>((stoul(s)));
	});
	ip_t expected = {uint8_t(192), uint8_t(168), uint8_t(0), uint8_t(1)};

	ASSERT_EQ(expected, result) ;
}


class FiltersTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
		for (size_t i = 0; i < POOL_SIZE; ++i)
		{
			ip_pool.emplace_back(otus::ip_t {
				(uint8_t) (i % MODULE),        (uint8_t) ((i + 5) % MODULE),
				(uint8_t) ((i + 7) % MODULE) , (uint8_t) ((i + 11) % MODULE)
			});
		}
	}

	std::vector<otus::ip_t> ip_pool;
	const size_t POOL_SIZE = 1024;
	const uint8_t b{42};
	const uint8_t MODULE = 255u;
};

TEST_F(FiltersTest, filter)
{
	std::vector<std::reference_wrapper<const otus::ip_t>> res = otus::filter(b, ip_pool);
	for (size_t i = 0; i < res.size(); ++i)
	{
		ASSERT_TRUE(res[i].get()[0] == b);
	}
}

TEST_F(FiltersTest, filter2)
{
	std::vector<std::reference_wrapper<const otus::ip_t>> res = otus::filter(b, b, ip_pool);
	for (size_t i = 0; i < res.size(); ++i)
	{
		ASSERT_TRUE(res[i].get()[0] == b && res[i].get()[1] == b);
	}
}

TEST_F(FiltersTest, filter_any)
{
	std::vector<std::reference_wrapper<const otus::ip_t>> res = otus::filter_any(b, ip_pool);
	for (size_t i = 0; i < res.size(); ++i)
	{
		ASSERT_TRUE(res[i].get()[0] == b ||
		            res[i].get()[1] == b ||
		            res[i].get()[2] == b ||
		            res[i].get()[3] == b);
	}
}

int main(int argc, char **argv)
{
	std::cout << "This is TEST RUNNER \\m/" << std::endl;
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
