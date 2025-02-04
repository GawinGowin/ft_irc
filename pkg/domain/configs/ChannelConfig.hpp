#ifndef CHANNELCONFIG_HPP
#define CHANNELCONFIG_HPP

#include <string>

struct ChannelConfig {
	std::string Name;
	std::string Topic;
	std::string Modes;
  // Key file, syntax for each line: "<user>:<nick>:<key>".
	std::string KeyFile;
};

#endif /* CHANNELCONFIG_HPP */
