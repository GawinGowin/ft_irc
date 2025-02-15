#include "domain/message/Parser.hpp"
#include <gtest/gtest.h>

TEST(ParserTest, testLongMessage) {
  // Max length of a message is 512 characters, including the CRLF(2 characters)
  // So, C is the last character of the message.
  const std::string strLong = "PRIVMSG #channel " + std::string(490, 'a') + "ABCDEFGH\r\n";
  Parser parser(strLong);

  EXPECT_EQ(parser.getParams()[1].back(), 'C');
}

TEST(ParserTest, testNoCRLF) {
  EXPECT_THROW(Parser("PRIVMSG #channel :Hello, world!"), std::runtime_error);
  EXPECT_THROW(Parser("\n"), std::runtime_error);
}

TEST(ParserTest, testPrefix) {
  Parser prefix(":prefix PRIVMSG #channel :Hello, world!\r\n");
  Parser noprefix("PRIVMSG #channel :Hello, world!\r\n");

  EXPECT_EQ(prefix.getPrefix(), "prefix");
  EXPECT_EQ(noprefix.getPrefix(), "");
  EXPECT_THROW(Parser(":prefix\r\n"), std::runtime_error);
  EXPECT_THROW(Parser(":prefix \r\n"), std::runtime_error);
}

TEST(ParserTest, testCommand) {
  Parser pass("PASS password\r\n");
  Parser nick("NICK nickname\r\n");
  Parser user("USER username hostname servername :realname\r\n");
  Parser join("JOIN #channel\r\n");
  Parser privmsg("PRIVMSG #channel :Hello, world!\r\n");
  Parser kick("KICK #channel user :reason\r\n");
  Parser invite("INVITE user #channel\r\n");
  Parser topic("TOPIC #channel :new topic\r\n");
  Parser mode("MODE #channel +o user\r\n");
  Parser error("ERROR :error message\r\n");
  Parser unknown("hoge huge\r\n");

  EXPECT_EQ(pass.getCommand(), IMessageAggregateRoot::PASS);
  EXPECT_EQ(nick.getCommand(), IMessageAggregateRoot::NICK);
  EXPECT_EQ(user.getCommand(), IMessageAggregateRoot::USER);
  EXPECT_EQ(join.getCommand(), IMessageAggregateRoot::JOIN);
  EXPECT_EQ(privmsg.getCommand(), IMessageAggregateRoot::PRIVMSG);
  EXPECT_EQ(kick.getCommand(), IMessageAggregateRoot::KICK);
  EXPECT_EQ(invite.getCommand(), IMessageAggregateRoot::INVITE);
  EXPECT_EQ(topic.getCommand(), IMessageAggregateRoot::TOPIC);
  EXPECT_EQ(mode.getCommand(), IMessageAggregateRoot::MODE);
  EXPECT_EQ(error.getCommand(), IMessageAggregateRoot::ERROR);
  EXPECT_EQ(unknown.getCommand(), IMessageAggregateRoot::UNKNOWN);
}
