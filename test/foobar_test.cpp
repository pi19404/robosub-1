#include "gtest/gtest.h"

TEST(FooBarTest, foo) {
  ASSERT_EQ(1, 1);
}

TEST(FooBarTest, bar) {
  ASSERT_EQ(1, 2);
}

