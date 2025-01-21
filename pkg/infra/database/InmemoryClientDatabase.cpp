#include "infra/database/InmemoryClientDatabase.hpp"

InmemoryClientDatabase::InmemoryClientDatabase(): _clients(std::vector<IClientAggregateRoot>()) {}

InmemoryClientDatabase::~InmemoryClientDatabase() {}

InmemoryClientDatabase::InmemoryClientDatabase(const InmemoryClientDatabase &other) {
  *this = other;
}

InmemoryClientDatabase &InmemoryClientDatabase::operator=(const InmemoryClientDatabase &other) {
  if (this != &other) {
    this->_clients = other._clients;
  }
}

void InmemoryClientDatabase::add(const IClientAggregateRoot &client) {
  this->_clients.push_back(client);
}

const std::vector<IClientAggregateRoot> &InmemoryClientDatabase::list() {
  return this->_clients;
}

const IClientAggregateRoot &InmemoryClientDatabase::getById(const int id) {
  std::vector<IClientAggregateRoot>::iterator it;
  for (it = this->_clients.begin(); it != this->_clients.end(); it++) {
    if (id == (*it).getId()) {
      return (*it);
    }
  }
  throw std::runtime_error("Not found");
}

void InmemoryClientDatabase::update(const int id, const IClientAggregateRoot &newData) {
  std::vector<IClientAggregateRoot>::iterator it;
  for (it = this->_clients.begin(); it != this->_clients.end(); it++) {
    if (id == (*it).getId()) {
      *it = newData;
      return ;
    }
  }
  throw std::runtime_error("Not found");
}

void InmemoryClientDatabase::remove(const IClientAggregateRoot &client) {
  std::vector<IClientAggregateRoot>::iterator it;
  it = std::find(this->_clients.begin(), this->_clients.end(), client);
  if (it != this->_clients.end()) {
    this->_clients.erase(it);
    return;
  }
  throw std::runtime_error("Not found");
}
