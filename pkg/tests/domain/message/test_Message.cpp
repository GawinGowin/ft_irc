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

TEST(MessageTest, ConstructorWithPrefixCommandAndParams) {
  Message message("nick!user@host", MessageConstants::PASS, "param1 param2 param3");

  EXPECT_EQ(message.getPrefix(), ":nick!user@host");
  EXPECT_EQ(message.getCommand(), MessageConstants::PASS);
  EXPECT_EQ(message.getParams().size(), 3);
  EXPECT_EQ(message.getParams()[0], "param1");
  EXPECT_EQ(message.getParams()[1], "param2");
  EXPECT_EQ(message.getParams()[2], "param3");
}

TEST(MessageTest, ConstructorWithPrefixAndResponseCode) {
  Message message("nick!user@host", 123, "param1 param2 param3");

  EXPECT_EQ(message.getPrefix(), ":nick!user@host");
  EXPECT_EQ(message.getCommand(), MessageConstants::UNDEFINED);
  EXPECT_EQ(message.getParams().size(), 3);
  EXPECT_EQ(message.getParams()[0], "param1");
  EXPECT_EQ(message.getParams()[1], "param2");
  EXPECT_EQ(message.getParams()[2], "param3");
  EXPECT_EQ(message.getNumericResponse(), "123");
  EXPECT_TRUE(message.isNumericResponse());
}

TEST(MessageTest, CopyConstructor) {
  Message original("nick!user@host", MessageConstants::PASS, "param1 param2 param3");
  Message copy(original);

  EXPECT_EQ(copy.getPrefix(), ":nick!user@host");
  EXPECT_EQ(copy.getCommand(), MessageConstants::PASS);
  EXPECT_EQ(copy.getParams().size(), 3);
  EXPECT_EQ(copy.getParams()[0], "param1");
  EXPECT_EQ(copy.getParams()[1], "param2");
  EXPECT_EQ(copy.getParams()[2], "param3");
}

TEST(MessageTest, AssignmentOperator) {
  Message original("nick!user@host", MessageConstants::PASS, "param1 param2 param3");
  Message assigned = original;

  EXPECT_EQ(assigned.getPrefix(), ":nick!user@host");
  EXPECT_EQ(assigned.getCommand(), MessageConstants::PASS);
  EXPECT_EQ(assigned.getParams().size(), 3);
  EXPECT_EQ(assigned.getParams()[0], "param1");
  EXPECT_EQ(assigned.getParams()[1], "param2");
  EXPECT_EQ(assigned.getParams()[2], "param3");
}

TEST(MessageTest, AssignmentOperatorWithNumericResponse) {
  Message original("server.example.com", 404, "nick :No such nick/channel");
  Message assigned;
  assigned = original;

  EXPECT_EQ(assigned.getPrefix(), ":server.example.com");
  EXPECT_TRUE(assigned.isNumericResponse());
  EXPECT_EQ(assigned.getNumericResponse(), "404");
  EXPECT_EQ(assigned.getParams().size(), 2);
  EXPECT_EQ(assigned.getParams()[0], "nick");
  EXPECT_EQ(assigned.getParams()[1], "No such nick/channel");
}

TEST(MessageTest, AssignmentOperatorSelfAssignment) {
  Message message("nick!user@host", MessageConstants::PRIVMSG, "target :Hello world!");
  
  // 自己代入のテスト - コンパイラ警告を抑制
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wself-assign-overloaded"
  message = message; // 自己代入
#pragma GCC diagnostic pop

  EXPECT_EQ(message.getPrefix(), ":nick!user@host");
  EXPECT_EQ(message.getCommand(), MessageConstants::PRIVMSG);
  EXPECT_EQ(message.getParams().size(), 2);
  EXPECT_EQ(message.getParams()[0], "target");
  EXPECT_EQ(message.getParams()[1], "Hello world!");
}

TEST(MessageTest, AssignmentOperatorEmptyMessage) {
  Message original;
  Message assigned("source", MessageConstants::JOIN, "#channel");
  assigned = original;

  EXPECT_EQ(assigned.getPrefix(), "");
  EXPECT_EQ(assigned.getCommand(), MessageConstants::UNDEFINED);
  EXPECT_EQ(assigned.getParams().size(), 0);
  EXPECT_FALSE(assigned.isNumericResponse());
}
