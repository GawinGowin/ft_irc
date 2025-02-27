#include "domain/channel/IChannelAggregateRoot.hpp"
#include "domain/shared/values/JenkinsHash.hpp"
#include "infra/database/InmemoryChannelDatabase.hpp"
#include <ctime>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

class MockChannelAggregateRoot : public IChannelAggregateRoot {
public:
  MOCK_METHOD(const std::string &, getName, (), (const, override));
  MOCK_METHOD(const JenkinsHash &, getId, (), (const, override));
  MOCK_METHOD(const int &, getModeFlags, (), (const, override));
  MOCK_METHOD(const time_t &, getCreationTime, (), (const, override));
  MOCK_METHOD(const std::string &, getTopic, (), (const, override));
  MOCK_METHOD(const unsigned long &, getMaxUsers, (), (const, override));
  MOCK_METHOD(const std::string &, getKey, (), (const, override));

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

  JenkinsHash id(this->channelName);

  EXPECT_CALL(this->mockChannel, getName()).WillRepeatedly(::testing::ReturnRef(this->channelName));
  EXPECT_CALL(*copyChannel, getName()).WillRepeatedly(::testing::ReturnRef(this->channelName));

  EXPECT_CALL(this->mockChannel, getId()).WillRepeatedly(::testing::ReturnRef(id));
  EXPECT_CALL(*copyChannel, getId()).WillRepeatedly(::testing::ReturnRef(id));
  EXPECT_CALL(this->mockChannel, clone()).WillRepeatedly(::testing::Return(copyChannel));

  this->database.add(this->mockChannel);

  auto db = database.getDatabase();
  ASSERT_EQ(db.size(), 1);
  EXPECT_EQ(db.begin()->first, this->channelName);
  EXPECT_EQ(db.begin()->second->getName(), this->channelName);

  auto result = this->database.get(this->channelName);
  ASSERT_NE(result, nullptr);
  EXPECT_EQ(result, copyChannel);
}

TEST_F(InmemoryChannelDatabaseTest, UpdateChannel) {
  MockChannelAggregateRoot *copyChannel;
  copyChannel = new MockChannelAggregateRoot();
  JenkinsHash id(this->channelName);

  EXPECT_CALL(this->mockChannel, getName()).WillRepeatedly(::testing::ReturnRef(this->channelName));
  EXPECT_CALL(this->mockChannel, getId()).WillRepeatedly(::testing::ReturnRef(id));
  EXPECT_CALL(this->mockChannel, clone()).WillRepeatedly(::testing::Return(copyChannel));
  EXPECT_CALL(*copyChannel, getName()).WillRepeatedly(::testing::ReturnRef(this->channelName));
  EXPECT_CALL(*copyChannel, getId()).WillRepeatedly(::testing::ReturnRef(id));

  MockChannelAggregateRoot mockChannelNew;
  std::string channelNameNew = "#NewChannelName";
  MockChannelAggregateRoot *copyNewChannel;
  copyNewChannel = new MockChannelAggregateRoot();
  JenkinsHash idNew(channelNameNew);

  EXPECT_CALL(mockChannelNew, getName()).WillRepeatedly(::testing::ReturnRef(channelNameNew));
  EXPECT_CALL(mockChannelNew, getId()).WillRepeatedly(::testing::ReturnRef(idNew));
  EXPECT_CALL(mockChannelNew, clone()).WillRepeatedly(::testing::Return(copyNewChannel));
  EXPECT_CALL(*copyNewChannel, getName()).WillRepeatedly(::testing::ReturnRef(channelNameNew));
  EXPECT_CALL(*copyNewChannel, getId()).WillRepeatedly(::testing::ReturnRef(idNew));

  // データベースに追加
  database.add(this->mockChannel);

  // 更新
  database.update(this->channelName, mockChannelNew);

  // 更新後の確認
  auto result = database.get(channelNameNew);
  ASSERT_NE(result, nullptr);
  EXPECT_EQ(result->getName(), channelNameNew);
}

TEST_F(InmemoryChannelDatabaseTest, RemoveChannel) {
  for (int i = 0; i < 1000; i++) {
    std::string channelName = "#Channel" + std::to_string(i);
    JenkinsHash id(channelName);

    MockChannelAggregateRoot mockChannel;
    MockChannelAggregateRoot *copyChannel;
    copyChannel = new MockChannelAggregateRoot();

    EXPECT_CALL(mockChannel, getName()).WillRepeatedly(::testing::ReturnRef(channelName));
    EXPECT_CALL(mockChannel, getId()).WillRepeatedly(::testing::ReturnRef(id));
    EXPECT_CALL(mockChannel, clone()).WillRepeatedly(::testing::Return(copyChannel));
    EXPECT_CALL(*copyChannel, getName()).WillRepeatedly(::testing::ReturnRef(channelName));
    EXPECT_CALL(*copyChannel, getId()).WillRepeatedly(::testing::ReturnRef(id));

    database.add(mockChannel);
    database.remove(channelName);
  }

  // 削除後の確認
  auto db = database.getDatabase();
  EXPECT_EQ(db.size(), 0);
  auto result = this->database.get(this->channelName);
  EXPECT_EQ(result, nullptr);
}
