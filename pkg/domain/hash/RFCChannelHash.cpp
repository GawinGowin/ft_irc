#include "domain/hash/RFCChannelHash.hpp"

static const std::string base = "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
static const int channelIdLength = 5;

RFCChannelHash::RFCChannelHash() : _hashedValue(0) {}

RFCChannelHash::RFCChannelHash(const std::time_t &current_time) {
  this->_hashedValue = static_cast<long>(current_time);
}

RFCChannelHash::RFCChannelHash(const RFCChannelHash &obj) { *this = obj; }

RFCChannelHash &RFCChannelHash::operator=(const RFCChannelHash &obj) {
  if (this != &obj) {
    this->_hashedValue = obj._hashedValue;
  }
  return *this;
}

RFCChannelHash::~RFCChannelHash() {}

bool RFCChannelHash::operator==(const IHashAggregateRoot &other) const {
  return this->_hashedValue == other.getHashLong();
}

bool RFCChannelHash::operator<(const IHashAggregateRoot &other) const {
  return this->_hashedValue < other.getHashLong();
}

std::string RFCChannelHash::getHash() const {
  int raw_id = this->_toHash(this->_hashedValue);
  size_t base_size = base.size();
  std::ostringstream oss;
  for (int i = 0; i < channelIdLength; ++i) {
    oss << base[raw_id % base_size];
    raw_id /= base_size;
  }
  return oss.str();
}

long RFCChannelHash::getHashLong() const { return this->_hashedValue; }

int RFCChannelHash::getHashInt() const { return this->_toHash(this->_hashedValue); }

int RFCChannelHash::_toHash(const std::time_t &current_time) const {
  const int base_size = static_cast<int>(base.size());
  long raw_id = current_time % static_cast<long>(std::pow(base_size, channelIdLength));
  return static_cast<int>(raw_id);
}
