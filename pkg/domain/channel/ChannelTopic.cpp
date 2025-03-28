#include "domain/channel/ChannelTopic.hpp"

ChannelTopic::ChannelTopic() {
  this->_topic = "";
  this->_topic_time = time(NULL);
  this->_topic_who = "";
}

ChannelTopic::ChannelTopic(const std::string &topic, const std::string &who) {
  this->_topic = topic;
  this->_topic_time = time(NULL);
  this->_topic_who = who;
}

ChannelTopic::~ChannelTopic() {}

ChannelTopic::ChannelTopic(const ChannelTopic &other) { *this = other; }

ChannelTopic &ChannelTopic::operator=(const ChannelTopic &other) {
  if (this != &other) {
    this->_topic = other._topic;
    this->_topic_time = other._topic_time;
    this->_topic_who = other._topic_who;
  }
  return *this;
}

void ChannelTopic::updateTopic(const std::string &topic, const std::string &who) {
  this->_topic = topic;
  this->_topic_time = time(NULL);
  this->_topic_who = who;
}

void ChannelTopic::clearTopic() { *this = ChannelTopic(); }

const std::string &ChannelTopic::getTopic() const { return this->_topic; }

const std::string &ChannelTopic::getWho() const { return this->_topic_who; }

const time_t &ChannelTopic::getWhen() const { return this->_topic_time; }
