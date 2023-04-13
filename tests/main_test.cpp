#include <gtest/gtest.h>
#include "func.cpp"

TEST(DEFAULT, 2isPrime)
{
    ASSERT_EQ(true, isPrime(2));
}
TEST(DEFAULT, 3isNotPrime)
{
    ASSERT_EQ(false, isPrime(3));
}
TEST(DEFAULT, 2plus5isEqual7)
{
    ASSERT_EQ(7, sum(2, 5));
    ASSERT_EQ(7, sum(5, 2));
}

  //TEST(DEFAULT, 1isEqual1)
  //{
  //    ASSERT_EQ(1, 1);

  //}