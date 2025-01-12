#include "ChannelMode.hpp"

ChannelMode::ChannelMode(): _mode(STANDARD_MODE), _flag(0) {}

ChannelMode::ChannelMode(EachChunnelMode mode): _mode(mode), _flag(0) {}

ChannelMode::~ChannelMode() {}

ChannelMode::ChannelMode(const ChannelMode &other) {
  *this = other;
}

ChannelMode &ChannelMode::operator=(const ChannelMode &other) {
  if (this != &other) {
    this->_mode = other._mode;
    this->_flag = other._flag;
  }
}

const EachChunnelMode ChannelMode::getMode() const {
    return this->_mode;
}

void ChannelMode::setMode(EachChunnelMode mode) {
  switch (mode) {
    case LOCAL_MODE:
      this->_mode = LOCAL_MODE;
      break;
    case ANONIMOUS_MODE:
      this->_mode = ANONIMOUS_MODE;
      break;
    case STANDARD_MODE:
      this->_mode = STANDARD_MODE;
      break;
    case MODE_DISABLED_MODE:
      this->_mode = MODE_DISABLED_MODE;
      break;
  }
  return ;
}

void ChannelMode::activateFlag(ChunnelFlag feature) {
  this->_flag |= feature;
  return ;
}

void ChannelMode::disableFlag(ChunnelFlag feature) {
  this->_flag &= ~feature;
  return ;
}
