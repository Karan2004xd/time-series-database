#include <gtest/gtest.h>
#include "../my_code.h"

TEST(AdditionTest, HandlesPositiveNumbers) {
  EXPECT_EQ(addTwoNumbers(10, 20), 30);
  EXPECT_EQ(addTwoNumbers(100, -20), 80);
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
