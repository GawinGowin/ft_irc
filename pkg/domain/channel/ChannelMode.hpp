#ifndef CHANNELMODE_HPP
#define CHANNELMODE_HPP

#include <list>
#include <string>

typedef std::list<std::string> SpecifiedChars;

enum ChannelInitialChar {
  CHANNEL_STANDARD = '#',
  CHANNEL_LOCAL = '&',
  CHANNEL_DISABLED = '+',
  CHANNEL_PRIVATE = '!'
};

class ChannelMode {
  private:
    const SpecifiedChars channelNameInitialChars({"#", "&", "+", "!"});
};

#endif /* CHANNELMODE_HPP */
