#include "domain/client/Password.hpp"
#include <gtest/gtest.h>

// デフォルトコンストラクタのテスト
TEST(PasswordTest, DefaultConstructor) {
  Password pwd;
  EXPECT_EQ(pwd.getPassword(), "");
}

// パラメータ付きコンストラクタのテスト
TEST(PasswordTest, ParameterizedConstructor) {
  Password pwd("mypassword");
  EXPECT_EQ(pwd.getPassword(), "mypassword");
}

// コピーコンストラクタのテスト
TEST(PasswordTest, CopyConstructor) {
  Password pwd1("mypassword");
  Password pwd2(pwd1);
  EXPECT_EQ(pwd2.getPassword(), "mypassword");
}

// 代入演算子のテスト
TEST(PasswordTest, AssignmentOperator) {
  Password pwd1("mypassword");
  Password pwd2;
  pwd2 = pwd1;
  EXPECT_EQ(pwd2.getPassword(), "mypassword");
}

// 等価演算子のテスト
TEST(PasswordTest, EqualityOperator) {
  Password pwd1("mypassword");
  Password pwd2("mypassword");
  EXPECT_TRUE(pwd1 == pwd2);

  Password pwd3("anotherpassword");
  EXPECT_FALSE(pwd1 == pwd3);
}

// setPasswordメソッドのテスト
TEST(PasswordTest, SetPassword) {
  Password pwd;
  EXPECT_EQ(pwd.setPassword("newpassword"), 0);
  EXPECT_EQ(pwd.getPassword(), "newpassword");
  EXPECT_EQ(pwd.setPassword("anotherpassword"), 1);
  EXPECT_EQ(pwd.getPassword(), "newpassword");
}
