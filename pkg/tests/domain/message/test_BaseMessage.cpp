#include "domain/message/BaseMessage.hpp"
#include <gtest/gtest.h>

TEST(BaseMessageTest, DefaultConstructor) {
  BaseMessage message;

  EXPECT_EQ(message.getPrefix(), "");
  EXPECT_EQ(message.getCommand(), IMessageAggregateRoot::UNKNOWN);
  EXPECT_EQ(message.getParams().size(), 0);
}

TEST(BaseMessageTest, Constructor) {
  BaseMessage message(":prefix PRIVMSG #channel :Hello, world!\r\n");

  EXPECT_EQ(message.getPrefix(), "prefix");
  EXPECT_EQ(message.getCommand(), IMessageAggregateRoot::PRIVMSG);
  EXPECT_EQ(message.getParams()[0], "#channel");
  EXPECT_EQ(message.getParams()[1], "Hello, world!");
}
