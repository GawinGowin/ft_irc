#ifndef CHANNELTOPIC_HPP
#define CHANNELTOPIC_HPP

#include <string>
#include <time.h>
#include <vector>

class ChannelTopic {
public:
  ChannelTopic();
  ChannelTopic(const std::string &topic, const std::string &who);
  ~ChannelTopic();
  ChannelTopic(const ChannelTopic &other);
  ChannelTopic &operator=(const ChannelTopic &other);

  void updateTopic(const std::string &topic, const std::string &who);
  void clearTopic();
  const std::string &getTopic() const;
  const std::string &getWho() const;
  const time_t &getWhen() const;

private:
  std::string _topic;     /* Channel topic */
  time_t _topic_time;     /* Time when topic was set */
  std::string _topic_who; /* Nickname of user that set topic */
};

#endif /* CHANNELTOPIC_HPP */
