#ifndef CHANNELMODE_HPP
#define CHANNELMODE_HPP

#include <list>
#include <string>

enum EachChunnelMode {
  LOCAL_MODE,
  ANONIMOUS_MODE,
  STANDARD_MODE,
  MODE_DISABLED_MODE,
};

/* 
O - give "channel creator" status;
o - give/take channel operator privilege;
v - give/take the voice privilege;

a - toggle the anonymous channel flag;
i - toggle the invite-only channel flag;
m - toggle the moderated channel;
n - toggle the no messages to channel from clients on the outside;
q - toggle the quiet channel flag;
p - toggle the private channel flag;
s - toggle the secret channel flag;
r - toggle the server reop channel flag;
t - toggle the topic settable by channel operator only flag;

k - set/remove the channel key (password);
l - set/remove the user limit to channel;

b - set/remove ban mask to keep users out;
e - set/remove an exception mask to override a ban mask;
I - set/remove an invitation mask to automatically override the invite-only flag;
 */
enum ChunnelFlag {
  CHANNEL_CREATOR = 1 << 0, // O - give "channel creator" status
  CHANNEL_OPERATOR = 1 << 1, // o - give/take channel operator privilege
  VOICE_PRIVILEGE = 1 << 2, // v - give/take the voice privilege

  ANONYMOUS_CHANNEL = 1 << 3, // a - toggle the anonymous channel flag
  INVITE_ONLY = 1 << 4, // i - toggle the invite-only channel flag
  MODERATED_CHANNEL = 1 << 5, // m - toggle the moderated channel
  NO_OUTSIDE_MESSAGES = 1 << 6, // n - toggle the no messages to channel from clients on the outside
  QUIET_CHANNEL = 1 << 7, // q - toggle the quiet channel flag
  PRIVATE_CHANNEL = 1 << 8, // p - toggle the private channel flag
  SECRET_CHANNEL = 1 << 9, // s - toggle the secret channel flag
  SERVER_REOP = 1 << 10, // r - toggle the server reop channel flag
  TOPIC_BY_OPERATOR_ONLY = 1 << 11, // t - toggle the topic settable by channel operator only flag

  CHANNEL_KEY = 1 << 12, // k - set/remove the channel key (password)
  USER_LIMIT = 1 << 13, // l - set/remove the user limit to channel

  BAN_MASK = 1 << 14, // b - set/remove ban mask to keep users out
  EXCEPTION_MASK = 1 << 15, // e - set/remove an exception mask to override a ban mask
  INVITATION_MASK = 1 << 16 // I - set/remove an invitation mask to automatically override the invite-only flag
};

class ChannelMode {
  public:
    ChannelMode();
    ChannelMode(EachChunnelMode mode);
    ~ChannelMode();
    ChannelMode(const ChannelMode &other);
    ChannelMode &operator=(const ChannelMode &other);

    const EachChunnelMode getMode() const;
    void setMode(EachChunnelMode mode);
    void activateFlag(ChunnelFlag flag);
    void disableFlag(ChunnelFlag feature);

  private:
    EachChunnelMode _mode;
    int _flag;
};

#endif /* CHANNELMODE_HPP */
