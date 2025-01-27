#ifndef CHANNELCLIENTLIST_HPP
#define CHANNELCLIENTLIST_HPP

#include <vector>

typedef int ClientUniqueID;

class ChannelClientList {
public:
  ChannelClientList();
  ~ChannelClientList();
  void addClient(const ClientUniqueID &id);
  void removeClient(const ClientUniqueID &id);
  bool isClientInList(const ClientUniqueID &id);
  std::vector<ClientUniqueID> &getClients();

private:
  std::vector<ClientUniqueID> _clients;
};

#endif /* CHANNELCLIENTLIST_HPP */
