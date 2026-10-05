#include "calc/calc.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <stdexcept>
#include <tuple>

using ::testing::ElementsAre;

// --- plain TEST()s ---------------------------------------------------------------

TEST(GcdTest, HandlesPositiveInput)
{
    EXPECT_EQ(calc::gcd(12, 18), 6);
    EXPECT_EQ(calc::gcd(17, 5), 1);
}

TEST(GcdTest, HandlesZeroAndNegativeInput)
{
    EXPECT_EQ(calc::gcd(0, 0), 0);
    EXPECT_EQ(calc::gcd(0, 7), 7);
    EXPECT_EQ(calc::gcd(-12, 18), 6);
}

TEST(FibonacciTest, KnownValues)
{
    EXPECT_EQ(calc::fibonacci(0), 0);
    EXPECT_EQ(calc::fibonacci(1), 1);
    EXPECT_EQ(calc::fibonacci(10), 55);
    EXPECT_EQ(calc::fibonacci(92), 7540113804746346429LL);
}

TEST(FibonacciTest, RejectsOutOfRange)
{
    EXPECT_THROW(calc::fibonacci(-1), std::out_of_range);
    EXPECT_THROW(calc::fibonacci(93), std::out_of_range);
}

// --- gMock matchers ----------------------------------------------------------------

TEST(SplitTest, SplitsOnSeparator)
{
    EXPECT_THAT(calc::split("a,b,c", ','), ElementsAre("a", "b", "c"));
    EXPECT_THAT(calc::split("a,,c", ','), ElementsAre("a", "", "c"));
    EXPECT_THAT(calc::split("", ','), ElementsAre(""));
}

// --- a value-parameterized test -------------------------------------------------------

class IsPrimeTest : public ::testing::TestWithParam<std::tuple<long long, bool>> {};

TEST_P(IsPrimeTest, ClassifiesCorrectly)
{
    const auto [n, expected] = GetParam();
    EXPECT_EQ(calc::is_prime(n), expected) << "n = " << n;
}

INSTANTIATE_TEST_SUITE_P(SmallNumbers, IsPrimeTest,
    ::testing::Values(std::make_tuple(-7LL, false), std::make_tuple(0LL, false),
                      std::make_tuple(1LL, false), std::make_tuple(2LL, true),
                      std::make_tuple(9LL, false), std::make_tuple(97LL, true),
                      std::make_tuple(7919LL, true), std::make_tuple(7921LL, false)));

// --- a fixture ----------------------------------------------------------------------

class SplitFixture : public ::testing::Test {
protected:
    void SetUp() override { fields = calc::split(line, ';'); }

    const std::string line = "id;name;email";
    std::vector<std::string> fields;
};

TEST_F(SplitFixture, KeepsFieldCount) { EXPECT_EQ(fields.size(), 3u); }

TEST_F(SplitFixture, KeepsFieldOrder)
{
    ASSERT_FALSE(fields.empty());
    EXPECT_EQ(fields.front(), "id");
    EXPECT_EQ(fields.back(), "email");
}
