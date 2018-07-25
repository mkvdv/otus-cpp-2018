#include "utils.h"

#include <gtest/gtest.h>


TEST(GtestSuiteMain, split)
{
	using namespace otus;
	std::string str = "192.168.0.1";
	std::vector<std::byte> result = split<std::byte>(str, '.', [](const std::string &s) {
			return static_cast<std::byte>((stoul(s)));});
	std::vector<std::byte> expected = {std::byte(192), std::byte(168), std::byte(0), std::byte(1)};
	
	ASSERT_EQ(expected, result) ;
}

int main(int argc, char **argv){
	std::cout << "This is TEST RUNNER \\m/" << std::endl;
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}