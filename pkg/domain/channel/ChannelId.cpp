#include "domain/channel/ChannelId.hpp"

static const std::string base = "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
static const int channelIdLength = 5;

ChannelId::ChannelId() : _channelid("00000") {}

ChannelId::ChannelId(const std::time_t &current_time) : _channelid(createChannelId(current_time)) {}

ChannelId::~ChannelId() {}

const std::string &ChannelId::getChannelId() const { return this->_channelid; }

bool ChannelId::operator==(const ChannelId &other) const {
  return this->_channelid == other._channelid;
}

const std::string ChannelId::createChannelId(const std::time_t &current_time) {
  const int base_size = base.size();
  std::time_t raw_id = current_time % static_cast<time_t>(std::pow(base_size, channelIdLength));
  std::ostringstream oss;
  for (int i = 0; i < channelIdLength; ++i) {
    oss << base[raw_id % base_size];
    raw_id /= base_size;
  }
  return oss.str();
}
