#include <gtest/gtest.h>
#include <iostream>

TEST (SimpleMessageTest, DirectOutput) {
    EXPECT_EQ(0, 0);
    std::cout << "Test works!" << std::endl;
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}