#include "ServerService.hpp"

void ServerService::registerClient(Server &svr, const IClientRepository &client) {
  try {
    svr.registerClientById(client.getId());
  } catch (std::runtime_error &e) {
    throw std::runtime_error(std::string("ServerService: ") + e.what());
  }
}

void ServerService::deleteClient(Server &svr, const IClientRepository &client) {
  try {
    svr.deleteClientById(client.getId());
  } catch (std::runtime_error &e) {
    throw std::runtime_error(std::string("ServerService: ") + e.what());
  }
}
