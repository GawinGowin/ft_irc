#include "domain/message/Message.hpp"
#include "domain/message/MessageConstants.hpp"
#include <gtest/gtest.h>

using namespace MessageConstants;

TEST(MessageTest, DefaultConstructor) {
  Message message;

  EXPECT_EQ(message.getPrefix(), "");
  EXPECT_EQ(message.getCommand(), MessageConstants::UNDEFINED);
  EXPECT_EQ(message.getParams().size(), 0);
}

TEST(MessageTest, Constructor) {
  Message message(":nick!user@host PASS your 1 1 :Current local users: 1, Max: 1\r\n");

  EXPECT_EQ(message.getPrefix(), ":nick!user@host");
  EXPECT_EQ(message.getParams()[0], "your");
  EXPECT_EQ(message.getParams()[1], "1");
  EXPECT_EQ(message.getParams()[2], "1");
  EXPECT_EQ(message.getParams()[3], ":Current local users: 1, Max: 1");
}
