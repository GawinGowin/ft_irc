#include "ChannelData.hpp"

static const std::string createChannelId();

ChannelData::ChannelData() : _name(""), _topic(""), _channelid("00000") {}

ChannelData::ChannelData(std::string name)
    : _name(name), _topic(""), _channelid(createChannelId()) {
  std::ostringstream oss;

  if (this->_name.empty()) {
    throw std::invalid_argument("Channel name cannot be empty");
  }
  if (this->_name.size() > max_name_length) {
    oss << "Channel name cannot be longer than " << max_name_length << " characters";
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

ChannelData::ChannelData(const ChannelData &other)
    : _name(other.getName() + "-copy"), _topic(""), _channelid(createChannelId()) {}

ChannelData &ChannelData::operator=(const ChannelData &other) { return *this; }

const std::string ChannelData::getName() const { return this->_name; }

const std::string ChannelData::getTopic() const { return this->_topic; }

const std::string ChannelData::getChannelId() const { return this->_channelid; }

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

static const std::string createChannelId() {
  const int base_size = base.size();
  const std::time_t current_time = std::time(nullptr);
  std::time_t raw_id = current_time % static_cast<time_t>(std::pow(base_size, channelIdLength));

  std::ostringstream oss;
  for (int i = 0; i < channelIdLength; ++i) {
    oss << base[raw_id % base_size];
    raw_id /= base_size;
  }
  return oss.str();
}