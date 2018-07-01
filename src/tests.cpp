#include "lib.h"

#include <gtest/gtest.h>


TEST(GtestSuiteMain, version)
{
    ASSERT_TRUE(version() > 0);
}

int main(int argc, char **argv){
	std::cout << "This is TEST RUNNER \\m/" << std::endl;
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
