#ifndef CHANNELCLIENTLIST_HPP
#define CHANNELCLIENTLIST_HPP

#include <algorithm>
#include <vector>

typedef long ClientUniqueID;

class ChannelClientList {
public:
  ChannelClientList();
  ~ChannelClientList();
  int addClient(const ClientUniqueID &id);
  int removeClient(const ClientUniqueID &id);
  std::vector<ClientUniqueID> &getClients();
  bool isClientInList(const ClientUniqueID &id);

private:
  std::vector<ClientUniqueID> _clients;
};

#endif /* CHANNELCLIENTLIST_HPP */
