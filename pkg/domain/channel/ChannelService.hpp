#ifndef CHANNELSERVICE_HPP
#define CHANNELSERVICE_HPP

#include "ChannelMode.hpp"
#include "ChannelData.hpp"

typedef std::list<std::string> CharsListSpecified;

class ChannelService {
  public:
    bool isValidChannel(const ChannelData &channelData);
    void setChannelMode(ChannelData &channelData, ChannelMode *mode);
    const std::string getErrorMsg() const;

  private:
    std::string _errorMsg;
};

#endif /* CHANNELSERVICE_HPP */
