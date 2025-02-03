#include "domain/channel/ChannelClientList.hpp"

ChannelClientList::ChannelClientList() : _clients() {}

ChannelClientList::~ChannelClientList() {}

int ChannelClientList::addClient(const ClientUniqueID &id) {
  std::vector<ClientUniqueID>::iterator it;
  it = std::find(_clients.begin(), _clients.end(), id);
  if (it == _clients.end()) {
    _clients.push_back(id);
    return 0;
  }
  return 1;
}

int ChannelClientList::removeClient(const ClientUniqueID &id) {
  std::vector<ClientUniqueID>::iterator it;
  it = std::find(_clients.begin(), _clients.end(), id);
  if (it != _clients.end()) {
    _clients.erase(it);
    return 0;
  }
  return 1;
}

bool ChannelClientList::isClientInList(const ClientUniqueID &id) {
  std::vector<ClientUniqueID>::iterator it;
  it = std::find(_clients.begin(), _clients.end(), id);
  return it != _clients.end();
}

std::vector<ClientUniqueID> &ChannelClientList::getClients() { return _clients; }
