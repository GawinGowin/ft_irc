#ifndef ICHANNELAGGREGATEROOT_HPP
#define ICHANNELAGGREGATEROOT_HPP

#include "domain/channel/ChannelClientList.hpp"
#include "domain/channel/ChannelId.hpp"
#include "domain/channel/ChannelTopic.hpp"
#include "domain/shared/values/IHashAggregateRoot.hpp"

#include <string>
#include <time.h>

class IChannelAggregateRoot {
public:
  virtual ~IChannelAggregateRoot(){};

  virtual const std::string &getName() const = 0;
  virtual const ChannelId &getId() const = 0;
  virtual const int &getModeFlags() const = 0;
  virtual const time_t &getCreationTime() const = 0;
  virtual const ChannelTopic &getTopic() const = 0;
  virtual const unsigned long &getMaxUsers() const = 0;
  virtual IHashAggregateRoot *getKey() const = 0;

  virtual void setModeFlags(int modeFlags) = 0;
  virtual void setMaxUsers(unsigned long maxusers) = 0;

  virtual IChannelAggregateRoot *clone() const = 0;

  virtual ChannelClientList &getListConnects() = 0;
  virtual ChannelClientList &getListBans() = 0;
  virtual ChannelClientList &getListExcepts() = 0;
  virtual ChannelClientList &getListInvites() = 0;
};

#endif /* ICHANNELAGGREGATEROOT_HPP */
