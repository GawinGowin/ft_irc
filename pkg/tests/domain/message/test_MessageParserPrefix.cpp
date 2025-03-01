#include "domain/message/Message.hpp"
#include <gtest/gtest.h>

struct TestData {
  std::string prefixString;
  std::string nick;
  std::string user;
  std::string host;
  int status;
};

std::ostream &operator<<(std::ostream &os, const TestData &data) {
  os << data.prefixString;
  return os;
}

class ParserPrefixTestNormal : public ::testing::TestWithParam<TestData> {};
class ParserPrefixTestAbnormal : public ::testing::TestWithParam<TestData> {};

TEST_P(ParserPrefixTestNormal, good) {
  auto param = GetParam();
  std::string prefix = param.prefixString;
  std::string expectedNick = param.nick;
  std::string expectedUser = param.user;
  std::string expectedHost = param.host;
  int expectStatus = param.status;

  PrefixInfo info;
  int status = Message::parsePrefixDetails(info, prefix);

  EXPECT_EQ(info.nick, expectedNick);
  EXPECT_EQ(info.user, expectedUser);
  EXPECT_EQ(info.host, expectedHost);
  EXPECT_EQ(status, expectStatus);
}

TEST_P(ParserPrefixTestAbnormal, bad) {
  auto param = GetParam();
  std::string prefix = param.prefixString;
  std::string expectedNick = param.nick;
  std::string expectedUser = param.user;
  std::string expectedHost = param.host;
  int expectStatus = param.status;

  PrefixInfo info;
  int status = Message::parsePrefixDetails(info, prefix);
  EXPECT_EQ(info.nick, expectedNick);
  EXPECT_EQ(info.user, expectedUser);
  EXPECT_EQ(info.host, expectedHost);
  EXPECT_EQ(status, expectStatus);
}

const TestData PrefixTestData[] = {
    {"nick!user@host", "nick", "user", "host", 0},
    {"nick@host", "nick", "", "host", 0},
    {"nick!user", "nick", "user", "", 0},
    {"nick", "nick", "", "", 0},
    {"server.example.com", "server.example.com", "", "", 0},
    {"nick-01!user_name@host.domain", "nick-01", "user_name", "host.domain", 0},
    {"[nick]!~user@192.168.1.1", "[nick]", "~user", "192.168.1.1", 0},
    {"nick\\!user@host", "nick\\", "user", "host", 0},
    {"nick!user^special@host-name", "nick", "user^special", "host-name", 0},
    {"nick!user@127.0.0.1", "nick", "user", "127.0.0.1", 0},
    {"nick!user@::1", "nick", "user", "::1", 0},
    {"nick!user@subdomain.example.com", "nick", "user", "subdomain.example.com", 0},
    {"nick123!user456@host789", "nick123", "user456", "host789", 0},
    {"nick_name!user-name@host_name", "nick_name", "user-name", "host_name", 0},
    {"nick!user@host:6667", "nick", "user", "host:6667", 0},
    {"nick!u@h", "nick", "u", "h", 0},
    {"nick name!user@host", "nick name", "user", "host", 0},
    {"nick!user name@host", "nick", "user name", "host", 0},
    {"nick!user@host name", "nick", "user", "host name", 0},
    {"nick\x01!user@host", "nick\x01", "user", "host", 0},
    {"nick!user\x02@host", "nick", "user\x02", "host", 0},
    {"nick!user@host\x03", "nick", "user", "host\x03", 0},
    {std::string(300, 'n') + "!user@host", std::string(300, 'n'), "user", "host", 0},
    {"nick!" + std::string(300, 'u') + "@host", "nick", std::string(300, 'u'), "host", 0},
    {"nick!user@" + std::string(300, 'h'), "nick", "user", std::string(300, 'h'), 0},
    {"ChanServ!service@services.local", "ChanServ", "service", "services.local", 0},
    {"irc.example.net", "irc.example.net", "", "", 0},
    {"nick!*@*", "nick", "*", "*", 0},
    {"*!user@host", "*", "user", "host", 0},
    {"*!*@*", "*", "*", "*", 0},
    {"nick♥!user@host", "nick♥", "user", "host", 0},
    {"nick!user♦@host", "nick", "user♦", "host", 0},
    {"nick!user@host♣", "nick", "user", "host♣", 0},
    {"nick<!user@host", "nick<", "user", "host", 0},
    {"nick!user>@host", "nick", "user>", "host", 0},
    {"nick!user@host&", "nick", "user", "host&", 0},
};

const TestData PrefixErrorTestData[] = {
    {"", "", "", "", 1},
    {"!user@host", "", "", "", 1},
    {"nick!@host", "", "", "", 1},
    {"nick!user@", "", "", "", 1},
    {"nick!user!extra@host", "", "", "", 1},
    {"nick!user@host@extra", "", "", "", 1},
    {"nick!user!name@host@domain", "", "", "", 1},
    {"nick!user@host.local@example", "", "", "", 1},
    {"!", "", "", "", 1},
    {"@", "", "", "", 1},
    {"!@", "", "", "", 1},
    {"@!", "", "", "", 1},
    {"nick@host!user", "", "", "", 1},
    {"!!@", "", "", "", 1},
    {"@@!", "", "", "", 1},
    {"n!!u@@h", "", "", "", 1},
    {"nick\\!name!user@host", "", "", "", 1},
    {"nick!user\\@name@host", "", "", "", 1},
};

INSTANTIATE_TEST_SUITE_P(
    ParserPrefixTests, ParserPrefixTestNormal, ::testing::ValuesIn(PrefixTestData));
INSTANTIATE_TEST_SUITE_P(
    ParserPrefixTests, ParserPrefixTestAbnormal, ::testing::ValuesIn(PrefixErrorTestData));
