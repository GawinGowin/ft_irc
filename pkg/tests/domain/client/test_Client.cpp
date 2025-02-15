#include "domain/client/Client.hpp"
#include <gtest/gtest.h>

const std::string TEST_NICKNAME = "TestNickName";
const int TEST_ID = 42;
const pollfd TEST_POLLFD = {42, POLLIN, 0};

TEST(ClientTest, SetAndGetTest) {
  Client client(TEST_ID, TEST_POLLFD);
  client.setNickName(TEST_NICKNAME);
  EXPECT_EQ(client.getId(), TEST_ID);
  EXPECT_EQ(client.getSocketFd(), TEST_POLLFD.fd);
  EXPECT_EQ(client.getPollfd().events, TEST_POLLFD.events);
  EXPECT_EQ(client.getPollfd().revents, TEST_POLLFD.revents);
  EXPECT_EQ(client.getNickName(), TEST_NICKNAME);
}
