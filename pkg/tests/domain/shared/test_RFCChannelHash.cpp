#include "domain/shared/values/RFCChannelHash.hpp"
#include <ctime>
#include <gtest/gtest.h>

TEST(RFCChannelHashTest, ConstructorTest) {
  std::time_t current_time = std::time(NULL);
  RFCChannelHash hash(current_time);
  ASSERT_EQ(hash.getHashLong(), static_cast<long>(current_time));
}

TEST(RFCChannelHashTest, CopyConstructorTest) {
  std::time_t current_time = std::time(NULL);
  RFCChannelHash hash1(current_time);
  RFCChannelHash hash2(hash1);
  ASSERT_EQ(hash1.getHashLong(), hash2.getHashLong());
}

TEST(RFCChannelHashTest, AssignmentOperatorTest) {
  std::time_t current_time = std::time(NULL);
  RFCChannelHash hash1(current_time);
  RFCChannelHash hash2 = hash1;
  ASSERT_EQ(hash1.getHashLong(), hash2.getHashLong());
}

TEST(RFCChannelHashTest, EqualityOperatorTest) {
  std::time_t current_time = std::time(NULL);
  RFCChannelHash hash1(current_time);
  RFCChannelHash hash2(current_time);
  ASSERT_TRUE(hash1 == hash2);
}

TEST(RFCChannelHashTest, GetHashTest) {
  std::time_t current_time = std::time(NULL);
  RFCChannelHash hash(current_time);
  std::string hash_str = hash.getHash();
  ASSERT_EQ(hash_str.length(), 5);
}

TEST(RFCChannelHashTest, GetHashLongTest) {
  std::time_t current_time = std::time(NULL);
  RFCChannelHash hash(current_time);
  ASSERT_EQ(hash.getHashLong(), static_cast<long>(current_time));
}

TEST(RFCChannelHashTest, GetHashIntTest) {
  std::time_t current_time = std::time(NULL);
  RFCChannelHash hash(current_time);
  int hash_int = hash.getHashInt();
  ASSERT_GE(hash_int, 0);
}
