#ifndef CHANNELCLIENTLIST_HPP
#define CHANNELCLIENTLIST_HPP

#include <vector>

typedef int ClientUniqueID;

class ChannelClientList {
public:
  ChannelClientList();
  ~ChannelClientList();
  void addClient(int clientUniqueID);
  void removeClient(int clientUniqueID);
  bool isClientInList(int clientUniqueID);
  std::vector<ClientUniqueID> getClients();

private:
  std::vector<ClientUniqueID> _clients;
};

#endif /* CHANNELCLIENTLIST_HPP */
