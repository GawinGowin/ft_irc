#include "ChanelInfo.hpp"

const std::list<std::string> forbiddenChars({":", ",", " ", "\a"});
const std::list<std::string> channelNameInitialChars({"#", "&", "+", "!"});

Channel::Channel(): _name(""), _topic("") {}

Channel::Channel(std::string name, std::string topic): _name(name), _topic(name) {
  if (this->_name.empty()) {
    throw std::invalid_argument("Channel name cannot be empty");
  }
  if (this->_name.size() > 50) {
    throw std::invalid_argument("Channel name cannot be longer than 50 characters");
  }
  if (this->_name.at(0) != '#' && this->_name.at(0) != '&' && this->_name.at(0) != '+' && this->_name.at(0) != '!') {
    throw std::invalid_argument("Channel name must start with #, &, + or !");
  }
  std::list<std::string>::const_iterator it;
  for (it = forbiddenChars.begin(); it != forbiddenChars.end(); ++it) {
    if (this->_name.find(*it) != std::string::npos) {
      throw std::invalid_argument("Channel name cannot contain ':', ',', ' ', or '\a'");
    }
  }
}

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
