#include "domain/channel/ChannelId.hpp"
#include "domain/channel/IChannelAggregateRoot.hpp"
#include "infra/database/InmemoryChannelDatabase.hpp"
#include <ctime>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

class MockChannelAggregateRoot : public IChannelAggregateRoot {
public:
  MOCK_METHOD(const std::string &, getName, (), (const, override));
  MOCK_METHOD(const ChannelId &, getId, (), (const, override));
  MOCK_METHOD(const int &, getModeFlags, (), (const, override));
  MOCK_METHOD(const time_t &, getCreationTime, (), (const, override));
  MOCK_METHOD(ChannelTopic &, getTopic, (), (const, override));
  MOCK_METHOD(const unsigned long &, getMaxUsers, (), (const, override));
  MOCK_METHOD(IHashAggregateRoot *, getKey, (), (const, override));

  MOCK_METHOD(void, setModeFlags, (int modeFlags), (override));
  MOCK_METHOD(void, setMaxUsers, (unsigned long maxusers), (override));

  MOCK_METHOD(IChannelAggregateRoot *, clone, (), (const, override));

  MOCK_METHOD(ChannelClientList &, getListConnects, (), (override));
  MOCK_METHOD(ChannelClientList &, getListBans, (), (override));
  MOCK_METHOD(ChannelClientList &, getListExcepts, (), (override));
  MOCK_METHOD(ChannelClientList &, getListInvites, (), (override));
};

class InmemoryChannelDatabaseTest : public ::testing::Test {
protected:
  InmemoryChannelDatabase database;
  MockChannelAggregateRoot mockChannel;

  std::string channelName = "#DefaultChannelName";
};

TEST_F(InmemoryChannelDatabaseTest, AddAndList) {
  MockChannelAggregateRoot *copyChannel;
  copyChannel =
      new MockChannelAggregateRoot(); // delete は不要 (InmemoryChannelDatabaseのデストラクタでdeleteされる)

  time_t current_time = time(NULL);
  ChannelId id(current_time);

  EXPECT_CALL(this->mockChannel, getName()).WillRepeatedly(::testing::ReturnRef(this->channelName));
  EXPECT_CALL(*copyChannel, getName()).WillRepeatedly(::testing::ReturnRef(this->channelName));

  EXPECT_CALL(this->mockChannel, getId()).WillRepeatedly(::testing::ReturnRef(id));
  EXPECT_CALL(*copyChannel, getId()).WillRepeatedly(::testing::ReturnRef(id));
  EXPECT_CALL(this->mockChannel, clone()).WillRepeatedly(::testing::Return(copyChannel));

  this->database.add(this->mockChannel);

  auto list = database.list();
  ASSERT_EQ(list.size(), 1);
  EXPECT_EQ(list[0]->getName(), channelName);
  EXPECT_EQ((list[0]->getId()).getChannelId(), current_time);

  auto result = this->database.get(id, this->channelName);
  ASSERT_NE(result, nullptr);
  EXPECT_EQ(result, copyChannel);
}

TEST_F(InmemoryChannelDatabaseTest, UpdateChannel) {
  time_t current_time = time(NULL);
  MockChannelAggregateRoot *copyChannel;
  copyChannel = new MockChannelAggregateRoot();
  ChannelId id(current_time);

  EXPECT_CALL(this->mockChannel, getName()).WillRepeatedly(::testing::ReturnRef(this->channelName));
  EXPECT_CALL(this->mockChannel, getId()).WillRepeatedly(::testing::ReturnRef(id));
  EXPECT_CALL(this->mockChannel, clone()).WillRepeatedly(::testing::Return(copyChannel));
  EXPECT_CALL(*copyChannel, getName()).WillRepeatedly(::testing::ReturnRef(this->channelName));
  EXPECT_CALL(*copyChannel, getId()).WillRepeatedly(::testing::ReturnRef(id));

  MockChannelAggregateRoot mockChannelNew;
  std::string channelNameNew = "#NewChannelName";
  MockChannelAggregateRoot *copyNewChannel;
  copyNewChannel = new MockChannelAggregateRoot();
  ChannelId idNew(current_time + 10000);

  EXPECT_CALL(mockChannelNew, getName()).WillRepeatedly(::testing::ReturnRef(channelNameNew));
  EXPECT_CALL(mockChannelNew, getId()).WillRepeatedly(::testing::ReturnRef(idNew));
  EXPECT_CALL(mockChannelNew, clone()).WillRepeatedly(::testing::Return(copyNewChannel));
  EXPECT_CALL(*copyNewChannel, getName()).WillRepeatedly(::testing::ReturnRef(channelNameNew));
  EXPECT_CALL(*copyNewChannel, getId()).WillRepeatedly(::testing::ReturnRef(idNew));

  // データベースに追加
  database.add(this->mockChannel);

  // 更新
  database.update(id, this->channelName, mockChannelNew);

  // 更新後の確認
  auto result = database.get(idNew, channelNameNew);
  ASSERT_NE(result, nullptr);
  EXPECT_EQ(result->getName(), channelNameNew);
}

TEST_F(InmemoryChannelDatabaseTest, RemoveChannel) {
  time_t current_time = time(NULL);
  MockChannelAggregateRoot *copyChannel;
  copyChannel = new MockChannelAggregateRoot();
  ChannelId id(current_time);

  EXPECT_CALL(this->mockChannel, getName()).WillRepeatedly(::testing::ReturnRef(this->channelName));
  EXPECT_CALL(this->mockChannel, getId()).WillRepeatedly(::testing::ReturnRef(id));
  EXPECT_CALL(this->mockChannel, clone()).WillRepeatedly(::testing::Return(copyChannel));
  EXPECT_CALL(*copyChannel, getName()).WillRepeatedly(::testing::ReturnRef(this->channelName));
  EXPECT_CALL(*copyChannel, getId()).WillRepeatedly(::testing::ReturnRef(id));

  // データベースに追加
  database.add(mockChannel);

  // 削除
  database.remove(id, this->channelName);

  // 削除後の確認
  auto list = database.list();
  EXPECT_EQ(list.size(), 0);
  EXPECT_EQ(database.get(id, this->channelName), nullptr);
}