#include "domain/message/Message.hpp"
#include <gtest/gtest.h>
#include <string>
#include <vector>

// Test structure to hold test cases
struct ParamTestCase {
  std::string paramStr;
  std::vector<std::string> expectedParams;
  std::string description;
};

class MessageParseParamsTest : public ::testing::TestWithParam<ParamTestCase> {};

TEST_P(MessageParseParamsTest, ParseParams) {
  auto testCase = GetParam();

  std::vector<std::string> params;
  int result = Message::parseParams(params, testCase.paramStr, NULL);

  EXPECT_EQ(result, 0) << "Failed on test case: " << testCase.description;
  EXPECT_EQ(params.size(), testCase.expectedParams.size())
      << "Param count mismatch on test case: " << testCase.description;

  for (size_t i = 0; i < params.size() && i < testCase.expectedParams.size(); ++i) {
    EXPECT_EQ(params[i], testCase.expectedParams[i])
        << "Param at index " << i << " mismatch on test case: " << testCase.description;
  }
}

// Individual test cases for specific scenarios
TEST(MessageParseParamsTest, EmptyString) {
  std::vector<std::string> params;
  std::string paramStr = "";

  int result = Message::parseParams(params, paramStr, NULL);

  EXPECT_EQ(result, 0);
  EXPECT_EQ(params.size(), 0);
}

TEST(MessageParseParamsTest, SingleParam) {
  std::vector<std::string> params;
  std::string paramStr = "param1";

  int result = Message::parseParams(params, paramStr, NULL);

  EXPECT_EQ(result, 0);
  EXPECT_EQ(params.size(), 1);
  EXPECT_EQ(params[0], "param1");
}

TEST(MessageParseParamsTest, MultipleParams) {
  std::vector<std::string> params;
  std::string paramStr = "param1 param2 param3";

  int result = Message::parseParams(params, paramStr, NULL);

  EXPECT_EQ(result, 0);
  EXPECT_EQ(params.size(), 3);
  EXPECT_EQ(params[0], "param1");
  EXPECT_EQ(params[1], "param2");
  EXPECT_EQ(params[2], "param3");
}

TEST(MessageParseParamsTest, TrailingParam) {
  std::vector<std::string> params;
  std::string paramStr = "param1 param2 :This is a trailing parameter";

  int result = Message::parseParams(params, paramStr, NULL);

  EXPECT_EQ(result, 0);
  EXPECT_EQ(params.size(), 3);
  EXPECT_EQ(params[0], "param1");
  EXPECT_EQ(params[1], "param2");
  EXPECT_EQ(params[2], "This is a trailing parameter");
}

TEST(MessageParseParamsTest, OnlyTrailingParam) {
  std::vector<std::string> params;
  std::string paramStr = ":This is only a trailing parameter";

  int result = Message::parseParams(params, paramStr, NULL);

  EXPECT_EQ(result, 0);
  EXPECT_EQ(params.size(), 1);
  EXPECT_EQ(params[0], "This is only a trailing parameter");
}

TEST(MessageParseParamsTest, TrailingParamWithColon) {
  std::vector<std::string> params;
  std::string paramStr = "param1 :This is a: trailing parameter with: colons";

  int result = Message::parseParams(params, paramStr, NULL);

  EXPECT_EQ(result, 0);
  EXPECT_EQ(params.size(), 2);
  EXPECT_EQ(params[0], "param1");
  EXPECT_EQ(params[1], "This is a: trailing parameter with: colons");
}

TEST(MessageParseParamsTest, ParamsWithCRLF) {
  std::vector<std::string> params;
  std::string paramStr = "param1 param2\r\n";

  int result = Message::parseParams(params, paramStr, NULL);

  EXPECT_EQ(result, 0);
  EXPECT_EQ(params.size(), 2);
  EXPECT_EQ(params[0], "param1");
  EXPECT_EQ(params[1], "param2");
}

TEST(MessageParseParamsTest, TrailingParamWithCRLF) {
  std::vector<std::string> params;
  std::string paramStr = "param1 :Trailing parameter\r\n";

  int result = Message::parseParams(params, paramStr, NULL);

  EXPECT_EQ(result, 0);
  EXPECT_EQ(params.size(), 2);
  EXPECT_EQ(params[0], "param1");
  EXPECT_EQ(params[1], "Trailing parameter");
}

TEST(MessageParseParamsTest, ClearsExistingParams) {
  std::vector<std::string> params = {"existing1", "existing2"};
  std::string paramStr = "param1 param2";

  int result = Message::parseParams(params, paramStr, NULL);

  EXPECT_EQ(result, 0);
  EXPECT_EQ(params.size(), 2);
  EXPECT_EQ(params[0], "param1");
  EXPECT_EQ(params[1], "param2");
}

TEST(MessageParseParamsTest, SpecialCharacters) {
  std::vector<std::string> params;
  std::string paramStr = "param1 param-2 param_3 :Special chars: !@#$%^&*()";

  int result = Message::parseParams(params, paramStr, NULL);

  EXPECT_EQ(result, 0);
  EXPECT_EQ(params.size(), 4);
  EXPECT_EQ(params[0], "param1");
  EXPECT_EQ(params[1], "param-2");
  EXPECT_EQ(params[2], "param_3");
  EXPECT_EQ(params[3], "Special chars: !@#$%^&*()");
}

TEST(MessageParseParamsTest, EmptyTrailingParam) {
  std::vector<std::string> params;
  std::string paramStr = "param1 param2 :";

  int result = Message::parseParams(params, paramStr, NULL);

  EXPECT_EQ(result, 0);
  EXPECT_EQ(params.size(), 3);
  EXPECT_EQ(params[0], "param1");
  EXPECT_EQ(params[1], "param2");
  EXPECT_EQ(params[2], "");
}

TEST(MessageParseParamsTest, WhitespaceHandling) {
  std::vector<std::string> params;
  std::string paramStr = "  param1   param2  param3  ";

  int result = Message::parseParams(params, paramStr, NULL);

  EXPECT_EQ(result, 0);
  EXPECT_EQ(params.size(), 3);
  EXPECT_EQ(params[0], "param1");
  EXPECT_EQ(params[1], "param2");
  EXPECT_EQ(params[2], "param3");
}

TEST(MessageParseParamsTest, WhitespaceInTrailingParam) {
  std::vector<std::string> params;
  std::string paramStr = "param1 :  Trailing with   multiple   spaces  ";

  int result = Message::parseParams(params, paramStr, NULL);

  EXPECT_EQ(result, 0);
  EXPECT_EQ(params.size(), 2);
  EXPECT_EQ(params[0], "param1");
  EXPECT_EQ(params[1], "  Trailing with   multiple   spaces  ");
}

// Parameterized test cases
INSTANTIATE_TEST_SUITE_P(
    MessageParseParamsTests,
    MessageParseParamsTest,
    ::testing::Values(
        ParamTestCase{"", {}, "Empty string"},
        ParamTestCase{"param1", {"param1"}, "Single parameter"},
        ParamTestCase{
            "param1 param2 param3", {"param1", "param2", "param3"}, "Multiple parameters"},
        ParamTestCase{
            "param1 param2 :Trailing param",
            {"param1", "param2", "Trailing param"},
            "With trailing parameter"},
        ParamTestCase{":Only trailing", {"Only trailing"}, "Only trailing parameter"},
        ParamTestCase{"param1 param2\r\n", {"param1", "param2"}, "With CRLF"},
        ParamTestCase{
            "param1 :Trailing with CRLF\r\n",
            {"param1", "Trailing with CRLF"},
            "Trailing parameter with CRLF"},
        ParamTestCase{"#channel +o user", {"#channel", "+o", "user"}, "Channel mode parameters"},
        ParamTestCase{
            "#channel :Welcome to IRC!", {"#channel", "Welcome to IRC!"}, "Channel topic"},
        ParamTestCase{
            "user :Ping timeout: 180 seconds",
            {"user", "Ping timeout: 180 seconds"},
            "Quit message"},
        ParamTestCase{
            "#channel user :You have been kicked",
            {"#channel", "user", "You have been kicked"},
            "Kick message"},
        ParamTestCase{
            "user #channel :You are invited",
            {"user", "#channel", "You are invited"},
            "Invite message"}));
