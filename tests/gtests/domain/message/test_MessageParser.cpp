#include "domain/message/Message.hpp"
#include "domain/message/MessageConstants.hpp"
#include <gtest/gtest.h>

using namespace MessageConstants;

TEST(ParserTest, testLongMessage) {
  // Max length of a message is 512 characters, including the CRLF(2 characters)
  // So, C is the last character of the message.
  const std::string strLong = "PRIVMSG #channel " + std::string(490, 'a') + "ABCDEFGH\r\n";
  Message parser(strLong);

  EXPECT_EQ(parser.getParams()[1].back(), 'C');
}

TEST(ParserTest, testNoCRLF) {
  Message nothing("");
  EXPECT_EQ(nothing.getCommand(), CommandType::ERROR);
  Message onlyLF("\n");
  EXPECT_EQ(onlyLF.getCommand(), CommandType::ERROR);
}

TEST(ParserTest, testPrefix) {
  Message noCRLF("PRIVMSG #channel :Hello, world!");
  Message prefix(":prefix PRIVMSG #channel :Hello, world!\r\n");
  Message noprefix("PRIVMSG #channel :Hello, world!\r\n");
  Message onlyPrefix(":prefix\r\n");
  Message onlyPrefixAndSpace(":prefix \r\n");

  EXPECT_EQ(noCRLF.getPrefix(), "");
  EXPECT_EQ(noCRLF.getCommand(), CommandType::PRIVMSG);
  EXPECT_EQ(noCRLF.getParams()[0], "#channel");
  EXPECT_EQ(noCRLF.getParams()[1], "Hello, world!");

  EXPECT_EQ(prefix.getPrefix(), ":prefix");
  EXPECT_EQ(prefix.getCommand(), CommandType::PRIVMSG);
  EXPECT_EQ(prefix.getParams()[0], "#channel");
  EXPECT_EQ(prefix.getParams()[1], "Hello, world!");
  EXPECT_EQ(noprefix.getPrefix(), "");
  EXPECT_EQ(onlyPrefix.getCommand(), CommandType::UNDEFINED);
  EXPECT_EQ(onlyPrefixAndSpace.getCommand(), CommandType::UNDEFINED);
}

TEST(ParserTest, testCommand) {
  Message pass("PASS password\r\n");
  Message nick("NICK nickname\r\n");
  Message user("USER username hostname servername :realname\r\n");
  Message join("JOIN #channel\r\n");
  Message privmsg("PRIVMSG #channel :Hello, world!\r\n");
  Message kick("KICK #channel user :reason\r\n");
  Message invite("INVITE user #channel\r\n");
  Message topic("TOPIC #channel :new topic\r\n");
  Message mode("MODE #channel +o user\r\n");
  Message error("ERROR\r\n");
  Message ERROR("hoge huge\r\n");
  Message partialMatch("MODECHANGE #channel +o user\r\n");

  EXPECT_EQ(pass.getCommand(), CommandType::PASS);
  EXPECT_EQ(nick.getCommand(), CommandType::NICK);
  EXPECT_EQ(user.getCommand(), CommandType::USER);
  EXPECT_EQ(join.getCommand(), CommandType::JOIN);
  EXPECT_EQ(privmsg.getCommand(), CommandType::PRIVMSG);
  EXPECT_EQ(kick.getCommand(), CommandType::KICK);
  EXPECT_EQ(invite.getCommand(), CommandType::INVITE);
  EXPECT_EQ(topic.getCommand(), CommandType::TOPIC);
  EXPECT_EQ(mode.getCommand(), CommandType::MODE);
  EXPECT_EQ(error.getCommand(), CommandType::ERROR);
  EXPECT_EQ(ERROR.getCommand(), CommandType::ERROR);
  EXPECT_EQ(partialMatch.getCommand(), CommandType::ERROR);
}

TEST(ParserTest, testParams) {
  Message noParams("PASS\r\n");
  Message noParamsAndSpace("PASS   \r\n");
  Message trailingParams("PRIVMSG #channel :Hello, world!\r\n");
  Message multipleParams("MODE #channel +o user  \r\n");

  EXPECT_EQ(noParams.getParams().size(), 0);
  EXPECT_EQ(noParamsAndSpace.getParams().size(), 0);
  EXPECT_EQ(trailingParams.getParams().size(), 2);
  EXPECT_EQ(multipleParams.getParams().size(), 3);
}
