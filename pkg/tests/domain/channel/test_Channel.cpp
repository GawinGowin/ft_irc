#include "domain/channel/Channel.hpp"
#include <gtest/gtest.h>

// テスト用の定数
const std::string TEST_CHANNEL_NAME = "TestChannel";
const unsigned long TEST_MAX_USERS = 100;

// コンストラクタの初期化テスト
TEST(ChannelTest, ConstructorInitialization) {
  Channel channel(TEST_CHANNEL_NAME);

  EXPECT_EQ(channel.getName(), TEST_CHANNEL_NAME);
  EXPECT_GT(channel.getCreationTime(), 0); // 現在時刻が正しく設定されているか
  EXPECT_EQ(channel.getModeFlags(), 0);    // 初期モードフラグが0であること
  EXPECT_EQ(channel.getMaxUsers(), DEFAULT_MAX_USERS);
  EXPECT_EQ(channel.getKey(), ""); // 初期キーが""であること
}

// クローン機能のテスト
TEST(ChannelTest, CloneCreatesIdenticalObject) {
  Channel channel(TEST_CHANNEL_NAME);
  Channel *clonedChannel = channel.clone();

  ASSERT_NE(clonedChannel, &channel); // 異なるインスタンスであることを確認
  EXPECT_EQ(clonedChannel->getName(), channel.getName());
  EXPECT_EQ(clonedChannel->getId(), channel.getId());
  EXPECT_EQ(clonedChannel->getModeFlags(), channel.getModeFlags());

  delete clonedChannel;
}

// setModeFlagsとgetModeFlagsのテスト
TEST(ChannelTest, SetAndGetModeFlags) {
  Channel channel(TEST_CHANNEL_NAME);
  int newModeFlags = 42;

  channel.setModeFlags(newModeFlags);
  EXPECT_EQ(channel.getModeFlags(), newModeFlags);
}

// setMaxUsersとgetMaxUsersのテスト
TEST(ChannelTest, SetAndGetMaxUsers) {
  Channel channel(TEST_CHANNEL_NAME);

  channel.setMaxUsers(TEST_MAX_USERS);
  EXPECT_EQ(channel.getMaxUsers(), TEST_MAX_USERS);
}

// チャネルリスト操作のテスト
TEST(ChannelTest, GetChannelClientLists) {
  Channel channel(TEST_CHANNEL_NAME);

  EXPECT_TRUE(channel.getListConnects().getClients().empty());
  EXPECT_TRUE(channel.getListBans().getClients().empty());
  EXPECT_TRUE(channel.getListExcepts().getClients().empty());
  EXPECT_TRUE(channel.getListInvites().getClients().empty());
}
