#include "domain/shared/values/SHAHash.hpp"
#include <gtest/gtest.h>

TEST(SHAHashTest, ConstructorTest) {
  std::string password = "test_password";
  SHAHash Password(password);
  ASSERT_FALSE(Password.getHash().empty());
}

TEST(SHAHashTest, CopyConstructorTest) {
  std::string password = "test_password";
  SHAHash Password1(password);
  SHAHash Password2(Password1);
  ASSERT_EQ(Password1.getHash(), Password2.getHash());
}

TEST(SHAHashTest, AssignmentOperatorTest) {
  std::string password1 = "test_password1";
  std::string password2 = "test_password2";
  SHAHash Password1(password1);
  SHAHash Password2(password2);
  Password2 = Password1;
  ASSERT_EQ(Password1.getHash(), Password2.getHash());
}

TEST(SHAHashTest, EqualityOperatorTest) {
  std::string password = "test_password";
  SHAHash Password1(password);
  SHAHash Password2(password);
  ASSERT_TRUE(Password1 == Password2);
}

TEST(SHAHashTest, GetHashTest) {
  std::string password = "test_password";
  SHAHash Password(password);
  std::string hash = Password.getHash();
  ASSERT_FALSE(hash.empty());
  ASSERT_EQ(hash.size(), 64); // SHA-256 hash length in hex is 64 characters
}

TEST(SHAHashTest, GetHashAsLongTest) {
  std::string password = "test_password1";
  SHAHash Password1(password);
  SHAHash Password2(Password1);
  long hash1 = Password1.getHashLong();
  long hash2 = Password2.getHashLong();
  ASSERT_FALSE(hash1 == 0);
  ASSERT_FALSE(hash2 == 0);
  ASSERT_EQ(hash1, hash2);
}

TEST(SHAHashTest, DifferentPasswordsTest) {
  std::string password1 = "test_password1";
  std::string password2 = "test_password2";
  SHAHash Password1(password1);
  SHAHash Password2(password2);
  ASSERT_FALSE(Password1 == Password2);
}
