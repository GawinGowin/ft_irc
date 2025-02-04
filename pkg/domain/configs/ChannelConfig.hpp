#ifndef CHANNELCONFIG_HPP
#define CHANNELCONFIG_HPP

#include <string>

struct ChannelConfig {
  std::string Name;
  std::string Topic;
  std::string Modes;
  // Key file, syntax for each line: "<user>:<nick>:<key>".
  std::string KeyFile;

  ChannelConfig() {}
  ChannelConfig(
      const std::string &Name,
      const std::string &Topic,
      const std::string &Modes,
      const std::string &KeyFile) {
    this->Name = Name;
    this->Topic = Topic;
    this->Modes = Modes;
    this->KeyFile = KeyFile;
  }
};

#endif /* CHANNELCONFIG_HPP */
