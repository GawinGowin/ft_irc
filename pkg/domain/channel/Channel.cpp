#include "Channel.hpp"

const ChannelData Channel::getChannelData() const { return this->_data; };

const ChannelMode Channel::getChannelMode() const { return *this->_mode; }