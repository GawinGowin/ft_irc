#ifndef CHANNELID_HPP
#define CHANNELID_HPP

#include <cmath>
#include <ctime>
#include <iostream>
#include <list>
#include <sstream>
#include <stdexcept>
#include <string>

class ChannelId {
public:
  ChannelId();
  ChannelId(const std::time_t &current_time);
  ~ChannelId();

  const std::string &getChannelId() const;
  const std::string &getChannelIdString() const;
  bool operator==(const ChannelId &other) const;

private:
  const std::string _channelid; // TODO: これを数値型に変更する
  const std::string createChannelId(const std::time_t &current_time);
};

#endif /* CHANNELID_HPP */
