#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "domain/channel/ChannelClientList.hpp"
#include "domain/channel/ChannelId.hpp"
#include "domain/channel/ChannelTopic.hpp"
#include "domain/channel/IChannelAggregateRoot.hpp"
#include "domain/shared/values/IPasswordAggregateRoot.hpp"
#include <string>
#include <time.h>

class Channel : virtual public IChannelAggregateRoot {
public:
  Channel(const std::string &name);
  ~Channel();

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

private:
  std::string _name;               /* Name of the channel */
  ChannelId _id;                   /* Unique channel ID */
  int _modeFlags;                  /* Channel modes */
  time_t _creation_time;           /* Channel creation time */
  ChannelTopic _topic;             /* Channel topic */
  IPasswordAggregateRoot *_key;    /* Channel key ("password", mode "k" ) */
  unsigned long _maxusers;         /* Maximum number of members (mode "l") */
  ChannelClientList _listConnects; /* list head of connected users */
  ChannelClientList _listBans;     /* list head of banned users */
  ChannelClientList _listExcepts;  /* list head of (ban) exception list */
  ChannelClientList _listInvites;  /* list head of invited users */
};

#endif /* CHANNEL_HPP */
