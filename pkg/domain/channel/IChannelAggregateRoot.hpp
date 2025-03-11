#ifndef ICHANNELAGGREGATEROOT_HPP
#define ICHANNELAGGREGATEROOT_HPP

#include "domain/channel/ChannelClientList.hpp"
#include "domain/channel/ChannelTopic.hpp"
#include "domain/shared/values/IHashAggregateRoot.hpp"

#include <string>
#include <time.h>

class IChannelAggregateRoot {
public:
  enum ChannelMode {
    MODE_INVITE_ONLY = 1 << 0,
    MODE_KEY_PROTECTED = 1 << 1,
    MODE_LIMIT_USERS = 1 << 2,
  };

  virtual ~IChannelAggregateRoot(){};

  virtual const std::string &getName() const = 0;
  virtual const IHashAggregateRoot &getId() const = 0;
  virtual const int &getModeFlags() const = 0;
  virtual const time_t &getCreationTime() const = 0;
  virtual const std::string &getTopic() const = 0;
  virtual const unsigned long &getMaxUsers() const = 0;
  virtual const std::string &getKey() const = 0;

  virtual void setModeFlags(int modeFlags) = 0;
  virtual void setMaxUsers(unsigned long maxusers) = 0;

  virtual IChannelAggregateRoot *clone() const = 0;

  virtual bool isKeyProtected() const = 0;
  virtual bool checkKey(const std::string &key) const = 0;
  virtual bool isMemberLimitExceeded() = 0;
  virtual bool isInviteOnly() const = 0;
  virtual bool isUserInvited(const std::string &nickname) = 0;

  virtual ChannelClientList &getListConnects() = 0;
  virtual ChannelClientList &getListBans() = 0;
  virtual ChannelClientList &getListExcepts() = 0;
  virtual ChannelClientList &getListInvites() = 0;

  virtual bool isOperator(const std::string &nickname) const = 0;
  virtual void addOperator(const std::string &nickname) = 0;
  virtual void removeOperator(const std::string &nickname) = 0;
};

#endif /* ICHANNELAGGREGATEROOT_HPP */
