#include "domain/channel/ChannelId.hpp"

static const std::string base = "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
static const int channelIdLength = 5;

ChannelId::ChannelId() : _channelidInt(0) {}

ChannelId::ChannelId(const std::time_t &current_time)
    : _channelidInt(static_cast<long>(current_time)) {}

ChannelId::~ChannelId() {}

ChannelId::ChannelId(const ChannelId &other) { *this = other; }

ChannelId &ChannelId::operator=(const ChannelId &other) {
  if (this != &other) {
    this->_channelidInt = other._channelidInt;
  }
  return *this;
}

const long &ChannelId::getChannelId() const { return this->_channelidInt; }

const std::string ChannelId::getChannelIdString() const {
  const int base_size = base.size();
  long raw_id = this->_channelidInt % static_cast<long>(std::pow(base_size, channelIdLength));
  std::ostringstream oss;
  for (int i = 0; i < channelIdLength; ++i) {
    oss << base[raw_id % base_size];
    raw_id /= base_size;
  }
  return oss.str();
}

bool ChannelId::operator==(const ChannelId &other) const {
  return this->_channelidInt == other._channelidInt;
}
