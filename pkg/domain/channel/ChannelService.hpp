#ifndef CHANNELSERVICE_HPP
#define CHANNELSERVICE_HPP

#include "ChannelMode.hpp"
#include "ChannelData.hpp"

typedef std::list<std::string> CharsListSpecified;

class ChannelService {
  public:
    void isValidChannel(const ChannelData &channelData);
    void setChannelMode(ChannelData &channelData, ChannelMode *mode);
};

#endif /* CHANNELSERVICE_HPP */
