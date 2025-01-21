#include "application/sample.hpp"
#include <gtest/gtest.h>

struct TestData {
  int input_a;
  int input_b;
  int expected_result;
};

std::ostream &operator<<(std::ostream &os, const TestData &data) {
  os << "\"" << data.input_a << "+" << data.input_b << "\""
     << " (correct answer: " << data.expected_result << ")";
  return os;
}

class SampleTest : public ::testing::TestWithParam<TestData> {};

TEST_P(SampleTest, Testcase) {
  auto param = GetParam();
  std::cout << "Input: \e[32m" << param.input_a << "," << param.input_b << "\e[m" << std::endl;
  int ret = add(param.input_a, param.input_b);

  const int expected = param.expected_result;
  std::cout << "Returned Value: \e[32m" << ret << "\e[m" << std::endl;
  std::cout << "Expected Value: \e[32m" << expected << "\e[m" << std::endl;
  EXPECT_EQ(ret, expected);
}

const TestData TestCases[] = {
    {1, 2, 3},  {2, 3, 5},  {3, 4, 7},  {4, 5, 9},   {5, 6, 11},
    {6, 7, 13}, {7, 8, 15}, {8, 9, 17}, {9, 10, 19}, {10, 11, 21},
};

INSTANTIATE_TEST_SUITE_P(
    AdTest,
    SampleTest,
    ::testing::ValuesIn(TestCases),
    [](const testing::TestParamInfo<SampleTest::ParamType> &info) {
      return "case" + std::to_string(info.index);
    });
