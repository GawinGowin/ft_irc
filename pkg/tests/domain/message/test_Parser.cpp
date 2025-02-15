#include "domain/message/Parser.hpp"
#include <gtest/gtest.h>

const std::string str_noprefix_trailing = "PRIVMSG #channel :Hello, world!\r\n";

TEST(ParserTest, testParseMessage) {
  Parser parser(str_noprefix_trailing);

  EXPECT_EQ(parser.getPrefix(), "");
  EXPECT_EQ(parser.getCommand(), IMessageAggregateRoot::PRIVMSG);
  EXPECT_EQ(parser.getParams().size(), 2);
}
