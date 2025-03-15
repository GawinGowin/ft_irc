#ifndef ICHANNELAGGREGATEROOT_HPP
#define ICHANNELAGGREGATEROOT_HPP

#include "domain/channel/ChannelClientList.hpp"
#include "domain/channel/ChannelTopic.hpp"
#include "domain/hash/IHashAggregateRoot.hpp"

#include <string>
#include <time.h>

class IChannelAggregateRoot {
public:
  enum ChannelMode {
    MODE_INVITE_ONLY = 1 << 0,      // +i, no args
    MODE_TOPIC_RESTRICTED = 1 << 1, // +t, no args
    MODE_KEY_PROTECTED = 1 << 2,    // +k, arg: key
    MODE_LIMIT_USERS = 1 << 3,      // +l, arg: limit
    MODE_CHANNEL_OPERATOR = 1 << 4, // +o, arg: nickname
  };

  virtual ~IChannelAggregateRoot(){};

  virtual const std::string &getName() const = 0;
  virtual const IHashAggregateRoot &getId() const = 0;
  virtual const int &getModeFlags() const = 0;
  virtual const time_t &getCreationTime() const = 0;
  virtual const std::string &getTopic() const = 0;
  virtual const std::string getMaxUsers() const = 0;
  virtual const std::string &getKey() const = 0;

  virtual void setKey(const std::string key) = 0;
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
