#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "domain/channel/ChannelClientList.hpp"
#include "domain/channel/ChannelTopic.hpp"
#include "domain/channel/IChannelAggregateRoot.hpp"
#include "domain/hash/JenkinsHash.hpp"
#include <string>
#include <time.h>

static const unsigned long DEFAULT_MAX_USERS = 5; // TODO: remove

class Channel : virtual public IChannelAggregateRoot {
public:
  Channel(const std::string &name);
  ~Channel();

  const std::string &getName() const;
  const JenkinsHash &getId() const;
  const int &getModeFlags() const;
  const time_t &getCreationTime() const;
  const std::string &getTopic() const;
  const std::string getMaxUsers() const;

  const std::string &getKey() const;
  void setKey(const std::string key);
  void setModeFlags(int modeFlags);
  void setMaxUsers(unsigned long maxusers);
  void updateTopic(const std::string &topic, const std::string &who);

  Channel *clone() const;

  ChannelClientList &getListConnects();
  ChannelClientList &getListBans();
  ChannelClientList &getListExcepts();
  ChannelClientList &getListInvites();

  bool isKeyProtected() const;
  bool checkKey(const std::string &key) const;
  bool isMemberLimitExceeded();
  bool isInviteOnly() const;
  bool isUserInvited(const std::string &nickname);

  bool isOperator(const std::string &nickname) const;
  void addOperator(const std::string &nickname);
  void removeOperator(const std::string &nickname);

private:
  std::string _name;                /* Name of the channel */
  JenkinsHash _id;                  /* Unique channel ID */
  int _modeFlags;                   /* Channel modes */
  time_t _creation_time;            /* Channel creation time */
  ChannelTopic _topic;              /* Channel topic */
  std::string _key;                 /* Channel key ("password", mode "k" ) */
  unsigned long _maxusers;          /* Maximum number of members (mode "l") */
  ChannelClientList _listConnects;  /* list head of connected users */
  ChannelClientList _listBans;      /* list head of banned users */
  ChannelClientList _listExcepts;   /* list head of (ban) exception list */
  ChannelClientList _listInvites;   /* list head of invited users */
  ChannelClientList _listOperators; /* list head of operators */
};

#endif /* CHANNEL_HPP */
