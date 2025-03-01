#include "domain/message/Message.hpp"
#include "domain/message/MessageConstants.hpp"
#include <gtest/gtest.h>

using namespace MessageConstants;

TEST(MessageTest, DefaultConstructor) {
  Message message;

  EXPECT_EQ(message.getPrefix(), "");
  EXPECT_EQ(message.getCommand(), CommandType::UNDEFINED);
  EXPECT_EQ(message.getParams().size(), 0);
}

TEST(MessageTest, Constructor) {
  Message message(":prefix PRIVMSG #channel :Hello, world!\r\n");

  EXPECT_EQ(message.getPrefix(), "prefix");
  EXPECT_EQ(message.getCommand(), CommandType::PRIVMSG);
  EXPECT_EQ(message.getParams()[0], "#channel");
  EXPECT_EQ(message.getParams()[1], "Hello, world!");
}
