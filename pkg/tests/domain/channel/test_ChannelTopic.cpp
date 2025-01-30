#include "domain/channel/ChannelTopic.hpp"
#include <gtest/gtest.h>

TEST(ChannelTopicTest, DefaultConstructor) {
  ChannelTopic topic;
  EXPECT_EQ(topic.getTopic(), "");
  EXPECT_EQ(topic.getWho(), 0);
  EXPECT_NE(topic.getWhen(), 0);
}

TEST(ChannelTopicTest, ParameterizedConstructor) {
  std::string test_topic = "Test Topic";
  ClientUniqueID test_who = 12345;
  ChannelTopic topic(test_topic, test_who);
  EXPECT_EQ(topic.getTopic(), test_topic);
  EXPECT_EQ(topic.getWho(), test_who);
  EXPECT_NE(topic.getWhen(), 0);
}

TEST(ChannelTopicTest, UpdateTopic) {
  ChannelTopic topic;
  std::string new_topic = "New Topic";
  ClientUniqueID new_who = 67890;
  topic.updateTopic(new_topic, new_who);
  EXPECT_EQ(topic.getTopic(), new_topic);
  EXPECT_EQ(topic.getWho(), new_who);
  EXPECT_NE(topic.getWhen(), 0);
}

TEST(ChannelTopicTest, ClearTopic) {
  std::string test_topic = "Test Topic";
  ClientUniqueID test_who = 12345;
  ChannelTopic topic(test_topic, test_who);
  topic.clearTopic();
  EXPECT_EQ(topic.getTopic(), "");
  EXPECT_EQ(topic.getWho(), 0);
  EXPECT_NE(topic.getWhen(), 0);
}

TEST(ChannelTopicTest, CopyConstructor) {
  std::string test_topic = "Test Topic";
  ClientUniqueID test_who = 12345;
  ChannelTopic original(test_topic, test_who);
  ChannelTopic copy(original);
  EXPECT_EQ(copy.getTopic(), test_topic);
  EXPECT_EQ(copy.getWho(), test_who);
  EXPECT_EQ(copy.getWhen(), original.getWhen());
}

TEST(ChannelTopicTest, AssignmentOperator) {
  std::string test_topic = "Test Topic";
  ClientUniqueID test_who = 12345;
  ChannelTopic original(test_topic, test_who);
  ChannelTopic assigned;
  assigned = original;
  EXPECT_EQ(assigned.getTopic(), test_topic);
  EXPECT_EQ(assigned.getWho(), test_who);
  EXPECT_EQ(assigned.getWhen(), original.getWhen());
}
