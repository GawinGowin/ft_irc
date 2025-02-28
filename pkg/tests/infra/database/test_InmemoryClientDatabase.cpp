#include "domain/client/IClientAggregateRoot.hpp"
#include "infra/database/InmemoryClientDatabase.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

class MockClient : public IClientAggregateRoot {
public:
  MockClient(std::string id, pollfd pfd) : _id(id), _pfd(pfd) {}
  MOCK_METHOD(const std::string &, getId, (), (const));
  MOCK_METHOD(const std::string &, getNickName, (), (const));
  MOCK_METHOD(const std::string &, getPassword, (), (const));
  MOCK_METHOD(const int &, getSocketFd, (), (const));
  MOCK_METHOD(const pollfd &, getPollfd, (), (const));
  MOCK_METHOD(const std::string &, getAddress, (), (const));

  MOCK_METHOD(void, setId, (const std::string &id), ());
  MOCK_METHOD(void, setNickName, (const std::string &nickName), ());
  MOCK_METHOD(int, setPassword, (const std::string &password), ());

  bool operator==(const IClientAggregateRoot &other) const override {
    return _id == other.getId() && _pfd.fd == other.getPollfd().fd &&
           _pfd.events == other.getPollfd().events && _pfd.revents == other.getPollfd().revents;
  }
  MOCK_METHOD(IClientAggregateRoot *, clone, (), (const));

private:
  std::string _id;
  pollfd _pfd;
};

TEST(InmemoryClientDatabaseTest, AddAndList) {
  InmemoryClientDatabase db;
  std::string id1 = "1";
  std::string id2 = "2";
  pollfd pfd1 = {1, POLLIN, 0};
  pollfd pfd2 = {2, POLLIN, 0};

  MockClient client1(id1, pfd1);
  MockClient *client1Copy = new MockClient(id1, pfd1);
  MockClient client2(id2, pfd2);
  MockClient *client2Copy = new MockClient(id2, pfd2);

  EXPECT_CALL(*client1Copy, getId()).WillRepeatedly(testing::ReturnRef(id1));
  EXPECT_CALL(*client2Copy, getId()).WillRepeatedly(testing::ReturnRef(id2));
  EXPECT_CALL(client1, clone()).WillRepeatedly(testing::Return(client1Copy));
  EXPECT_CALL(client2, clone()).WillRepeatedly(testing::Return(client2Copy));

  db.add(client1);
  db.add(client2);

  EXPECT_EQ(db.size(), 2);
  const auto &clients = db.list();
  ASSERT_EQ(clients.size(), 2);
  EXPECT_EQ(clients[0]->getId(), "1");
  EXPECT_EQ(clients[1]->getId(), "2");
}

TEST(InmemoryClientDatabaseTest, GetById) {
  InmemoryClientDatabase db;
  std::string id1 = "1";
  std::string id2 = "2";
  pollfd pfd1 = {1, POLLIN, 0};
  pollfd pfd2 = {2, POLLIN, 0};

  MockClient client1(id1, pfd1);
  MockClient *client1Copy = new MockClient(id1, pfd1);
  MockClient client2(id2, pfd2);
  MockClient *client2Copy = new MockClient(id2, pfd2);

  EXPECT_CALL(*client1Copy, getId()).WillOnce(testing::ReturnRef(id1));
  EXPECT_CALL(*client2Copy, getId()).WillRepeatedly(testing::ReturnRef(id2));
  EXPECT_CALL(client1, clone()).WillRepeatedly(testing::Return(client1Copy));
  EXPECT_CALL(client2, clone()).WillRepeatedly(testing::Return(client2Copy));

  db.add(client1);
  db.add(client2);

  const auto retrievedClient = db.getById(id2);
  EXPECT_EQ(retrievedClient->getId(), id2);
}

TEST(InmemoryClientDatabaseTest, Update) {
  InmemoryClientDatabase db;
  std::string id1 = "1";
  pollfd pfd1 = {1, POLLIN, 0};
  MockClient client1(id1, pfd1);
  MockClient *client1Copy = new MockClient(id1, pfd1);
  MockClient updatedClient1(id1, pfd1);
  MockClient *updatedClient1Copy = new MockClient(id1, pfd1);

  EXPECT_CALL(*client1Copy, getId()).WillRepeatedly(testing::ReturnRef(id1));
  EXPECT_CALL(client1, clone()).WillRepeatedly(testing::Return(client1Copy));
  EXPECT_CALL(*updatedClient1Copy, getId()).WillRepeatedly(testing::ReturnRef(id1));
  EXPECT_CALL(updatedClient1, clone()).WillRepeatedly(testing::Return(updatedClient1Copy));

  db.add(client1);
  db.update(id1, updatedClient1);

  const auto retrievedClient = db.getById(id1);
  EXPECT_EQ(retrievedClient->getId(), id1);
}

TEST(InmemoryClientDatabaseTest, Remove) {
  InmemoryClientDatabase db;
  std::string id1 = "1";
  std::string id2 = "2";
  pollfd pfd1 = {1, POLLIN, 0};
  pollfd pfd2 = {2, POLLIN, 0};

  MockClient client1(id1, pfd1);
  MockClient *client1Copy = new MockClient(id1, pfd1);
  MockClient client2(id2, pfd2);
  MockClient *client2Copy = new MockClient(id2, pfd2);

  EXPECT_CALL(*client1Copy, getId()).WillOnce(testing::ReturnRef(id1));
  EXPECT_CALL(*client2Copy, getId()).WillRepeatedly(testing::ReturnRef(id2));
  EXPECT_CALL(client1, clone()).WillRepeatedly(testing::Return(client1Copy));
  EXPECT_CALL(client2, clone()).WillRepeatedly(testing::Return(client2Copy));

  db.add(client1);
  db.add(client2);
  db.remove(id1);

  EXPECT_EQ(db.size(), 1);
  const auto &clients = db.list();
  ASSERT_EQ(clients.size(), 1);
  EXPECT_EQ(clients[0]->getId(), "2");
}

TEST(InmemoryClientDatabaseTest, Clear) {
  InmemoryClientDatabase db;
  std::string id1 = "1";
  std::string id2 = "2";
  pollfd pfd1 = {1, POLLIN, 0};
  pollfd pfd2 = {2, POLLIN, 0};

  MockClient client1(id1, pfd1);
  MockClient *client1Copy = new MockClient(id1, pfd1);
  MockClient client2(id2, pfd2);
  MockClient *client2Copy = new MockClient(id2, pfd2);

  EXPECT_CALL(client1, clone()).WillRepeatedly(testing::Return(client1Copy));
  EXPECT_CALL(client2, clone()).WillRepeatedly(testing::Return(client2Copy));

  db.add(client1);
  db.add(client2);
  db.clear();

  EXPECT_EQ(db.size(), 0);
  const auto &clients = db.list();
  EXPECT_TRUE(clients.empty());
}

TEST(InmemoryClientDatabaseTest, ListPollfds) {
  InmemoryClientDatabase db;
  std::string id1 = "1";
  std::string id2 = "2";
  pollfd pfd1 = {1, POLLIN, 0};
  pollfd pfd2 = {2, POLLIN, 0};

  MockClient client1(id1, pfd1);
  MockClient *client1Copy = new MockClient(id1, pfd1);
  MockClient client2(id2, pfd2);
  MockClient *client2Copy = new MockClient(id2, pfd2);

  EXPECT_CALL(client1, clone()).WillRepeatedly(testing::Return(client1Copy));
  EXPECT_CALL(client2, clone()).WillRepeatedly(testing::Return(client2Copy));

  EXPECT_CALL(*client1Copy, getPollfd()).WillRepeatedly(testing::ReturnRef(pfd1));
  EXPECT_CALL(*client2Copy, getPollfd()).WillRepeatedly(testing::ReturnRef(pfd2));

  db.add(client1);
  db.add(client2);

  const auto &pollfds = db.listPollfds();
  ASSERT_EQ(pollfds.size(), 2);
  EXPECT_EQ(pollfds[0].fd, 1);
  EXPECT_EQ(pollfds[1].fd, 2);
  EXPECT_EQ(pollfds[0].events, POLLIN);
  EXPECT_EQ(pollfds[1].events, POLLIN);
  EXPECT_EQ(pollfds[0].revents, 0);
  EXPECT_EQ(pollfds[1].revents, 0);
}

TEST(InmemoryClientDatabaseTest, Size) {
  InmemoryClientDatabase db;
  std::string id1 = "1";
  std::string id2 = "2";
  pollfd pfd1 = {1, POLLIN, 0};
  pollfd pfd2 = {2, POLLIN, 0};

  MockClient client1(id1, pfd1);
  MockClient *client1Copy = new MockClient(id1, pfd1);
  MockClient client2(id2, pfd2);
  MockClient *client2Copy = new MockClient(id2, pfd2);

  EXPECT_CALL(*client1Copy, getId()).WillRepeatedly(testing::ReturnRef(id1));
  EXPECT_CALL(*client2Copy, getId()).WillRepeatedly(testing::ReturnRef(id2));
  EXPECT_CALL(client1, clone()).WillRepeatedly(testing::Return(client1Copy));
  EXPECT_CALL(client2, clone()).WillRepeatedly(testing::Return(client2Copy));

  EXPECT_EQ(db.size(), 0);

  db.add(client1);
  EXPECT_EQ(db.size(), 1);

  db.add(client2);
  EXPECT_EQ(db.size(), 2);

  db.remove(id1);
  EXPECT_EQ(db.size(), 1);

  db.clear();
  EXPECT_EQ(db.size(), 0);
}
