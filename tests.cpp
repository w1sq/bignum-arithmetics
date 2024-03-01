#include <gtest/gtest.h>
#include <long_arithmetics.hpp>

using namespace LongArithmetics;

// Test for constructor that takes a user literal
TEST(BigNumConstructor, LiteralConstructor)
{
    BigNum a = 0.0_bn;
    EXPECT_EQ(to_string(a), "0");
}

// Test for constructor that takes an int
TEST(BigNumConstructor, IntConstructor)
{
    BigNum a(123.456);
    EXPECT_EQ(to_string(a), "123.456");
}

// Test for constructor that takes a string
TEST(BigNumConstructor, StringConstructor)
{
    BigNum a("00123.1010");
    EXPECT_EQ(to_string(a), "123.101");
}