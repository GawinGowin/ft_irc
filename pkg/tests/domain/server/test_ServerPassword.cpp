#include "domain/server/ServerPassword.hpp"
#include <gtest/gtest.h>

TEST(ServerPasswordTest, ConstructorTest) {
  std::string password = "test_password";
  ServerPassword serverPassword(password);
  ASSERT_FALSE(serverPassword.getHash().empty());
}

TEST(ServerPasswordTest, CopyConstructorTest) {
  std::string password = "test_password";
  ServerPassword serverPassword1(password);
  ServerPassword serverPassword2(serverPassword1);
  ASSERT_EQ(serverPassword1.getHash(), serverPassword2.getHash());
}

TEST(ServerPasswordTest, AssignmentOperatorTest) {
  std::string password1 = "test_password1";
  std::string password2 = "test_password2";
  ServerPassword serverPassword1(password1);
  ServerPassword serverPassword2(password2);
  serverPassword2 = serverPassword1;
  ASSERT_EQ(serverPassword1.getHash(), serverPassword2.getHash());
}

TEST(ServerPasswordTest, EqualityOperatorTest) {
  std::string password = "test_password";
  ServerPassword serverPassword1(password);
  ServerPassword serverPassword2(password);
  ASSERT_TRUE(serverPassword1 == serverPassword2);
}

TEST(ServerPasswordTest, HashFunctionTest) {
  std::string password = "test_password";
  ServerPassword serverPassword(password);
  std::string hash = serverPassword.getHash();
  ASSERT_FALSE(hash.empty());
  ASSERT_EQ(hash.size(), 64); // SHA-256 hash length in hex is 64 characters
}

TEST(ServerPasswordTest, DifferentPasswordsTest) {
  std::string password1 = "test_password1";
  std::string password2 = "test_password2";
  ServerPassword serverPassword1(password1);
  ServerPassword serverPassword2(password2);
  ASSERT_FALSE(serverPassword1 == serverPassword2);
}
