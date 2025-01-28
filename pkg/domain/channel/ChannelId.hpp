#ifndef CHANNELID_HPP
#define CHANNELID_HPP

#include <cmath>
#include <ctime>
#include <sstream>
#include <string>

class ChannelId {
public:
  ChannelId();
  ChannelId(const std::time_t &current_time);
  ~ChannelId();
  ChannelId(const ChannelId &other);
  ChannelId &operator=(const ChannelId &other);

  const long &getChannelId() const;
  const std::string getChannelIdString() const;
  bool operator==(const ChannelId &other) const;

private:
  long _channelidInt;
};

#endif /* CHANNELID_HPP */
