#include "infra/database/InmemoryClientDatabase.hpp"

InmemoryClientDatabase::InmemoryClientDatabase(): _clients(std::vector<Client>()) {}

InmemoryClientDatabase::~InmemoryClientDatabase() {}

InmemoryClientDatabase::InmemoryClientDatabase(const InmemoryClientDatabase &other) {
  *this = other;
}

InmemoryClientDatabase &InmemoryClientDatabase::operator=(const InmemoryClientDatabase &other) {
  if (this != &other) {
    this->_clients = other._clients;
  }
}

void InmemoryClientDatabase::addClient(const Client &client) { this->_clients.push_back(client); }

const std::vector<Client> &InmemoryClientDatabase::getClients() { return this->_clients; }

const Client &InmemoryClientDatabase::getClientById(const int id) {
  std::vector<Client>::iterator it;
  for (it = this->_clients.begin(); it != this->_clients.end(); it++) {
    if (id == (*it).getId()) {
      return (*it);
    }
  }
  throw std::runtime_error("Client not found");
}

void InmemoryClientDatabase::popClientById(const int id) {
  std::vector<Client>::iterator it;
  for (it = this->_clients.begin(); it != _clients.end(); it++) {
    if (id == (*it).getId()) {
      _clients.erase(it);
      return;
    }
  }
  throw std::runtime_error("Client not found");
}
