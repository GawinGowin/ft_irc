#include "ChannelData.hpp"

static const std::string createChannelId();

ChannelData::ChannelData() : _name(""), _topic(""), _channelid("00000") {}

ChannelData::ChannelData(std::string name)
    : _name(name), _topic(""), _channelid(createChannelId()) {}

ChannelData::ChannelData(const ChannelData &other)
    : _name(other.getName() + "-copy"), _topic(""), _channelid(createChannelId()) {}

ChannelData &ChannelData::operator=(const ChannelData &other) { return *this; }

const std::string ChannelData::getName() const { return this->_name; }

const std::string ChannelData::getTopic() const { return this->_topic; }

const std::string ChannelData::getChannelId() const { return this->_channelid; }

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