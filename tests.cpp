#include <long_arithmetics.hpp>

#include <gtest/gtest.h>

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

// Test for addition of two BigNumbers
TEST(BigNumAddition, AddTwoBigNumbers)
{
    BigNum bn1(123456789);
    BigNum bn2(987654321);
    BigNum result = bn1 + bn2;
    EXPECT_EQ(to_string(result), "1111111110");
}

// Test for addition of two BigNumbers with different signs
TEST(BigNumAddition, AddTwoBigNumbersDiffSigns)
{
    BigNum bn1(-123456789);
    BigNum bn2(987654321);
    BigNum result = bn1 + bn2;
    EXPECT_EQ(to_string(result), "864197532");
}

// Test for substraction of two BigNumbers
TEST(BigNumSubstraction, SubTwoBigNumbers)
{
    BigNum bn1(123456789);
    BigNum bn2(987654321);
    BigNum result = bn1 - bn2;
    EXPECT_EQ(to_string(result), "-864197532");
}

// Test for substraction of two BigNumbers with different signs
TEST(BigNumSubstraction, SubTwoBigNumbersDiffSigns)
{
    BigNum bn1(123456789);
    BigNum bn2(-987654321);
    BigNum result = bn1 - bn2;
    EXPECT_EQ(to_string(result), "1111111110");
}

// Test for multiplication of two BigNumbers
TEST(BigNumMultiplication, MulTwoBigNumbers)
{
    BigNum bn1(123456);
    BigNum bn2(987654);
    BigNum result = bn1 * bn2;
    EXPECT_EQ(to_string(result), "121931812224");
}

// Test for division of two BigNumbers
TEST(BigNumDivision, DivTwoBigNumbers)
{
    BigNum bn1(123456789);
    BigNum bn2(987654321);
    BigNum result = bn1 / bn2;
    EXPECT_EQ(to_string(result).substr(0, 13), "0.12499999886");
}

// Test for comparision of two equal BigNumbers
TEST(BigNumComparision, CompareTwoBigNumbers)
{
    BigNum a("8765432.10");
    BigNum b("8765432.10");
    EXPECT_EQ(a == b, true);
    EXPECT_EQ(a != b, false);
    EXPECT_EQ(a < b, false);
    EXPECT_EQ(a > b, false);
    EXPECT_EQ(a <= b, true);
    EXPECT_EQ(a >= b, true);
}

// Test for comparision of two diff BigNumbers
TEST(BigNumComparision, CompareTwoEQBigNumbers)
{
    BigNum a("-987654.32");
    BigNum b("456789.98");

    EXPECT_EQ(a == b, false);
    EXPECT_EQ(a != b, true);
    EXPECT_EQ(a < b, true);
    EXPECT_EQ(a > b, false);
    EXPECT_EQ(a <= b, true);
    EXPECT_EQ(a >= b, false);
}