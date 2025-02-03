#ifndef CHANNELTOPIC_HPP
#define CHANNELTOPIC_HPP

#include <string>
#include <time.h>
#include <vector>

typedef long ClientUniqueID;

class ChannelTopic {
public:
  ChannelTopic();
  ChannelTopic(const std::string &topic, const ClientUniqueID &who);
  ~ChannelTopic();
  ChannelTopic(const ChannelTopic &other);
  ChannelTopic &operator=(const ChannelTopic &other);

  void updateTopic(const std::string &topic, const ClientUniqueID &who);
  void clearTopic();
  const std::string &getTopic() const;
  const ClientUniqueID &getWho() const;
  const time_t &getWhen() const;

private:
  std::string _topic;        /* Channel topic */
  time_t _topic_time;        /* Time when topic was set */
  ClientUniqueID _topic_who; /* Nickname of user that set topic */
};

#endif /* CHANNELTOPIC_HPP */
