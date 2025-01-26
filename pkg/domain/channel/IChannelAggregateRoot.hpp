#ifndef ICHANNELAGGREGATEROOT_HPP
#define ICHANNELAGGREGATEROOT_HPP

#include "domain/channel/ChannelClientList.hpp"
#include "domain/channel/ChannelTopic.hpp"
#include "domain/shared/values/IPasswordAggregateRoot.hpp"

#include <string>
#include <time.h>

class IChannelAggregateRoot {
public:
  IChannelAggregateRoot(const std::string &name);
  ~IChannelAggregateRoot();

  const std::string &getName() const;
  int getModeFlags() const;
  time_t getCreationTime() const;
  ChannelTopic &getTopic() const;
  unsigned long getMaxUsers() const;
  IPasswordAggregateRoot *getKey() const;

  void setModeFlags(int modeFlags);
  void setMaxUsers(unsigned long maxusers);

  ChannelClientList &getListConnects();
  ChannelClientList &getListBans();
  ChannelClientList &getListExcepts();
  ChannelClientList &getListInvites();
};

#endif /* ICHANNELAGGREGATEROOT_HPP */
