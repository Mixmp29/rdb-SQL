#include <gtest/gtest.h>
#include <sum.hpp>

#include <iostream>

TEST(SumTest, SumDouble)
{
    EXPECT_FLOAT_EQ(sum(0.1, 0.2), 0.3);
}
