#include "domain/shared/values/JenkinsHash.cpp"
#include <gtest/gtest.h>

TEST(JenkinsHashTest, ConstructorTest) {
  std::string password = "test_password";
  JenkinsHash Password(password);
  ASSERT_FALSE(Password.getHash().empty());
}

TEST(JenkinsHashTest, CopyConstructorTest) {
  std::string password = "test_password";
  JenkinsHash Password1(password);
  JenkinsHash Password2(Password1);
  ASSERT_EQ(Password1.getHash(), Password2.getHash());
}

TEST(JenkinsHashTest, AssignmentOperatorTest) {
  std::string password1 = "test_password1";
  std::string password2 = "test_password2";
  JenkinsHash Password1(password1);
  JenkinsHash Password2(password2);
  Password2 = Password1;
  ASSERT_EQ(Password1.getHash(), Password2.getHash());
}

TEST(JenkinsHashTest, EqualityOperatorTest) {
  std::string password = "test_password";
  JenkinsHash Password1(password);
  JenkinsHash Password2(password);
  ASSERT_TRUE(Password1 == Password2);
}

TEST(JenkinsHashTest, GetHashTest) {
  std::string password = "test_password";
  JenkinsHash Password(password);
  std::string hash = Password.getHash();
  ASSERT_FALSE(hash.empty());
  ASSERT_EQ(hash.size(), 8); // JenkinsHash hash is 32 bits long (int size)
}

TEST(JenkinsHashTest, GetHashAsLongTest) {
  std::string password = "test_password1";
  JenkinsHash Password1(password);
  JenkinsHash Password2(Password1);
  long hash1 = Password1.getHashLong();
  long hash2 = Password2.getHashLong();
  ASSERT_FALSE(hash1 == 0);
  ASSERT_FALSE(hash2 == 0);
  ASSERT_EQ(hash1, hash2);
}

TEST(JenkinsHashTest, DifferentPasswordsTest) {
  std::string password1 = "test_password1";
  std::string password2 = "test_password2";
  JenkinsHash Password1(password1);
  JenkinsHash Password2(password2);
  ASSERT_FALSE(Password1 == Password2);
}
