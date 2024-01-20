#include <gtest/gtest.h>

int mainProgram();

TEST(MainProgramTest, ReturnsZero) {
    EXPECT_EQ(mainProgram(), 0);
}
