#include "ServerService.hpp"

void ServerService::addClient(Server &svr, const IClientRepository &client) {
  try {
    svr.addClientById(client.getId());
  } catch (std::runtime_error &e) {
    throw std::runtime_error(std::string("ServerService: ") + e.what());
  }
}

void ServerService::removeClient(Server &svr, const IClientRepository &client) {
  try {
    svr.removeClientById(client.getId());
  } catch (std::runtime_error &e) {
    throw std::runtime_error(std::string("ServerService: ") + e.what());
  }
}
