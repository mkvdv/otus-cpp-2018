#include "utils.h"

#include <gtest/gtest.h>


TEST(GtestSuiteMain, split)
{
	using namespace std;
	string str = "192.168.0.1";
	vector<byte> result = split<byte>(str, '.', [](const string &s) {
			return static_cast<byte>((stoul(s)));});
	vector<byte> expected = {byte(192), byte(168), byte(0), byte(1)};
	
	ASSERT_EQ(expected, result) ;
}

int main(int argc, char **argv){
	std::cout << "This is TEST RUNNER \\m/" << std::endl;
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}