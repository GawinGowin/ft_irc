#include "presentation/entrypoint.hpp"
#include "gtest/gtest.h"
#include <stdexcept>

class EntrypointTest : public ::testing::Test {
protected:
  void SetUp() override {
    // Setup code if needed
  }

  void TearDown() override {
    // Cleanup code if needed
  }
};

TEST_F(EntrypointTest, TestEntrypointInitialization) {
  const int argc = 3;
  char *argv[] = {(char *)"./ft_irc", (char *)"8080", (char *)"password", nullptr};
  EXPECT_NO_THROW({ entrypoint(argc, argv); });
}

TEST_F(EntrypointTest, TestEntrypointRuntimeError) {
  const int argc = 2;
  char *argv[] = {(char *)"./ft_irc", (char *)"8080", nullptr};
  EXPECT_THROW({ entrypoint(argc, argv); }, std::runtime_error);
}

TEST_F(EntrypointTest, TestEntrypointInfiniteLoop) {
  const int argc = 3;
  char *argv[] = {(char *)"./ft_irc", (char *)"8080", (char *)"password", nullptr};
  // This test is to ensure the function enters the loop, but we can't actually run it indefinitely.
  // We will simulate the loop entry and exit after one iteration.
  // This requires modifying the entrypoint function to allow breaking the loop for testing purposes.
  // For now, we will just check if it starts without throwing.
  EXPECT_NO_THROW({ entrypoint(argc, argv); });
}