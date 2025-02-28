#include "domain/channel/ChannelClientList.hpp"
#include <algorithm>

ChannelClientList::ChannelClientList() : _clients() {}

ChannelClientList::~ChannelClientList() {}

int ChannelClientList::addClient(const std::string &nickname) {
  std::vector<std::string>::iterator it;
  it = std::find(_clients.begin(), _clients.end(), nickname);
  if (it == _clients.end()) {
    _clients.push_back(nickname);
    return 0;
  }
  return 1;
}

int ChannelClientList::removeClient(const std::string &nickname) {
  std::vector<std::string>::iterator it;
  it = std::find(_clients.begin(), _clients.end(), nickname);
  if (it != _clients.end()) {
    _clients.erase(it);
    return 0;
  }
  return 1;
}

bool ChannelClientList::isClientInList(const std::string &nickname) {
  std::vector<std::string>::iterator it;
  it = std::find(_clients.begin(), _clients.end(), nickname);
  return it != _clients.end();
}

bool ChannelClientList::hasClient(const std::string &nickname) const {
  return std::find(_clients.begin(), _clients.end(), nickname) != _clients.end();
}

std::vector<std::string> &ChannelClientList::getClients() { return _clients; }
