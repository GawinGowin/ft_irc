#include "domain/channel/ChannelId.hpp"
#include <ctime>
#include <gtest/gtest.h>

TEST(ChannelIdTest, DefaultConstructor) {
  ChannelId channelId;
  EXPECT_EQ(channelId.getChannelId(), 0);
}

TEST(ChannelIdTest, ParameterizedConstructor) {
  std::time_t currentTime = std::time(nullptr);
  ChannelId channelId(currentTime);
  EXPECT_EQ(channelId.getChannelId(), static_cast<long>(currentTime));
}

TEST(ChannelIdTest, GetChannelIdString) {
  std::time_t currentTime = std::time(nullptr);
  ChannelId channelId(currentTime);
  std::string channelIdStr = channelId.getChannelIdString();
  EXPECT_EQ(channelIdStr.length(), 5); // 5 is the hyperparameter
  for (char c : channelIdStr) {
    EXPECT_TRUE(isalnum(c));
  }
}

TEST(ChannelIdTest, EqualityOperator) {
  std::time_t currentTime = std::time(nullptr);
  ChannelId channelId1(currentTime);
  ChannelId channelId2(currentTime);
  EXPECT_TRUE(channelId1 == channelId2);

  ChannelId channelId3(currentTime + 1);
  EXPECT_FALSE(channelId1 == channelId3);
}
