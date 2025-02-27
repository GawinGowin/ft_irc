#include "application/dto/StartServerDTO.hpp"
#include <gtest/gtest.h>
#include <stdexcept>

struct TestData {
  std::vector<std::string> argv;
};

std::ostream &operator<<(std::ostream &os, const TestData &data) {
  std::copy(data.argv.begin(), data.argv.end(), std::ostream_iterator<std::string>(os, ", "));
  return os;
}

class PositiveArgsTest : public ::testing::TestWithParam<TestData> {};
class NegativeArgsTest : public ::testing::TestWithParam<TestData> {};

TEST_P(PositiveArgsTest, Positive) {
  auto param = GetParam();

  std::vector<const char *> c_args;
  for (const auto &arg : param.argv) {
    c_args.push_back(arg.c_str());
  }
  c_args.push_back(nullptr);
  const char **const_argv = c_args.data();
  char **argv = const_cast<char **>(const_argv);
  int argc = c_args.size() - 1;
  StartServerDTO dto(argc, argv);
  EXPECT_FALSE(dto.isError());
}

TEST_P(NegativeArgsTest, Negative) {
  auto param = GetParam();
  std::vector<const char *> c_args;
  for (const auto &arg : param.argv) {
    c_args.push_back(arg.c_str());
  }
  c_args.push_back(nullptr);
  const char **const_argv = c_args.data();
  char **argv = const_cast<char **>(const_argv);
  int argc = c_args.size() - 1;
  StartServerDTO dto(argc, argv);
  EXPECT_TRUE(dto.isError());
}

const TestData PositiveTestData[] = {
    {{"./ft_irc", "9090", "admin"}},
    {{"./ft_irc", "7070", "root"}},
    {{"./ft_irc", "6060", "toor"}},
    {{"./ft_irc", "5050", "123456"}},
    {{"./ft_irc", "4040", "password123"}},
    {{"./ft_irc", "3030", "password123456"}},
    {{"./ft_irc", "2020", "password_123456789"}},
};

const TestData NegativeTestData[] = {
    {{"./ft_irc", "8080"}},
    {{"./ft_irc", "80a0", "password"}},
    {{"./ft_irc", "08080", "password"}},
    {{"./ft_irc", "70000", "password"}},
    {{"./ft_irc", "8080", ""}},
    {{"./ft_irc", "8080", "pass word"}}, // パスワードにスペースは含められない(ngircdの仕様)
    {{"./ft_irc", "8080", "password", "extra"}},
    {{"./ft_irc"}},
    {{"./ft_irc", "8080a", "p@sswArd"}},
    {{"./ft_irc", "8080", "password", "extra"}},
    {{"./ft_irc"}},
    {{"./ft_irc", "8080a", "p@sswArd"}},
};

INSTANTIATE_TEST_SUITE_P(
    PositiveDataTest,
    PositiveArgsTest,
    ::testing::ValuesIn(PositiveTestData),
    [](const testing::TestParamInfo<PositiveArgsTest::ParamType> &info) {
      return std::to_string(info.index);
    });

INSTANTIATE_TEST_SUITE_P(
    NegativeDataTest,
    NegativeArgsTest,
    ::testing::ValuesIn(NegativeTestData),
    [](const testing::TestParamInfo<NegativeArgsTest::ParamType> &info) {
      return std::to_string(info.index);
    });
