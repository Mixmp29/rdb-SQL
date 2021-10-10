#include <gtest/gtest.h>
#include <iostream>
#include <sum.hpp>

TEST(SumTest, SumDouble)
{
    EXPECT_EQ(sum(1, 2), 3);
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}