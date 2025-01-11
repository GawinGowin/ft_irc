#include "ChanelInfo.hpp"

Channel::Channel(): _name(""), _topic("") {}

Channel::Channel(std::string name, std::string topic): _name(name), _topic(name) {}

Channel::Channel(const Channel &other) {
  *this = other;
}

Channel &Channel::operator=(const Channel &other) {
  if (this != &other) {
    this->_name = other._name;
    this->_topic = other._topic;
  }
  return *this;
}

std::string Channel::getName() const {
  return this->_name;
}

std::string Channel::getTopic() const {
  return this->_topic;
}
