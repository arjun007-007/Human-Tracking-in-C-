/**
 * @file main.cpp
 * @author Lowell Lobo
 * @author Mayank Deshpande
 * @author Kautilya Chappidi
 * @brief main testing implementation
 * @version 0.1
 * @date 2023-10-24
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <gtest/gtest.h>

/**
 * @brief Function to initialise and run Google Tests
 *
 * @param argc
 * @param argv
 * @return int
 */
int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
