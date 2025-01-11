#include "ChanelInfo.hpp"

Channel::Channel() : _name(""), _mode("") {}

Channel::Channel(std::string name) : _name(name), _mode(std::string(1, name[0])) {
  std::ostringstream oss;

  if (this->_name.empty()) {
    throw std::invalid_argument("Channel name cannot be empty");
  }
  if (this->_name.size() > max_length) {
    oss << "Channel name cannot be longer than " << max_length << " characters";
    throw std::invalid_argument(oss.str());
  }
  std::list<std::string>::const_iterator it;
  for (it = channelNameInitialChars.begin(); it != channelNameInitialChars.end(); ++it) {
    if (this->_name[0] == (*it)[0]) {
      break;
    }
    oss << "Channel name must start with " << channelNameInitialChars;
    throw std::invalid_argument(oss.str());
  }
  for (it = forbiddenChars.begin(); it != forbiddenChars.end(); ++it) {
    if (this->_name.find(*it) != std::string::npos) {
      oss << "Channel name cannot contain " << forbiddenChars;
      throw std::invalid_argument(oss.str());
    }
  }
}

Channel::Channel(const Channel &other) : _name(other.getName()), _mode(other.getMode()) {}

Channel &Channel::operator=(const Channel &other) { return *this; }

std::string Channel::getName() const { return this->_name; }

std::string Channel::getMode() const { return this->_mode; }

std::ostream &operator<<(std::ostream &os, const CharsListSpecified &lst) {
  if (lst.empty()) {
    return os;
  }
  std::list<std::string>::const_iterator it = lst.begin();
  os << "'" << *it << "'";
  ++it;

  for (; it != lst.end(); ++it) {
    if (std::distance(it, lst.end()) == 1) {
      os << " or ";
    } else {
      os << ", ";
    }
    os << "'" << *it << "'";
  }
  return os;
}
