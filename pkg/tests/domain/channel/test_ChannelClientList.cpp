#include "domain/channel/ChannelClientList.hpp"
#include <gtest/gtest.h>

TEST(ChannelClientListTest, AddClient) {
  ChannelClientList clientList;
  std::string id1 = "1";
  std::string id2 = "2";

  EXPECT_EQ(clientList.addClient(id1), 0);
  EXPECT_EQ(clientList.addClient(id1), 1); // Adding the same client should return 1
  EXPECT_EQ(clientList.addClient(id2), 0);
}

TEST(ChannelClientListTest, RemoveClient) {
  ChannelClientList clientList;
  std::string id1 = "1";
  std::string id2 = "2";

  clientList.addClient(id1);
  clientList.addClient(id2);

  EXPECT_EQ(clientList.removeClient(id1), 0);
  EXPECT_EQ(clientList.removeClient(id1), 1); // Removing the same client again should return 1
  EXPECT_EQ(clientList.removeClient(id2), 0);
}

TEST(ChannelClientListTest, IsClientInList) {
  ChannelClientList clientList;
  std::string id1 = "1";
  std::string id2 = "2";

  clientList.addClient(id1);

  EXPECT_TRUE(clientList.isClientInList(id1));
  EXPECT_FALSE(clientList.isClientInList(id2));
}

TEST(ChannelClientListTest, GetClients) {
  ChannelClientList clientList;
  std::string id1 = "1";
  std::string id2 = "2";

  clientList.addClient(id1);
  clientList.addClient(id2);

  std::vector<std::string> clients = clientList.getClients();
  EXPECT_EQ(clients.size(), 2);
  EXPECT_EQ(clients[0], id1);
  EXPECT_EQ(clients[1], id2);
}
