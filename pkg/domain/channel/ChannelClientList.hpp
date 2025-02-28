#ifndef CHANNELCLIENTLIST_HPP
#define CHANNELCLIENTLIST_HPP

#include <algorithm>
#include <string>
#include <vector>

class ChannelClientList {
public:
  ChannelClientList();
  ~ChannelClientList();
  int addClient(const std::string &nickname);
  int removeClient(const std::string &nickname);
  bool isClientInList(const std::string &nickname);
  std::vector<std::string> &getClients();
  bool hasClient(const std::string &nickname) const;

private:
  std::vector<std::string> _clients;
};

#endif /* CHANNELCLIENTLIST_HPP */
