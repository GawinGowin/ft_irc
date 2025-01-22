#include "infra/database/InmemoryClientDatabase.hpp"

InmemoryClientDatabase::InmemoryClientDatabase(): _clients(std::vector<IClientAggregateRoot*>()), _cachedPollfds(std::vector<pollfd>()), _isPollfdsCached(false) {}

InmemoryClientDatabase::~InmemoryClientDatabase() {
  std::vector<IClientAggregateRoot*>::iterator it;
  for (it = this->_clients.begin(); it != this->_clients.end(); it++) {
    delete *it;
  }
  this->_clients.clear();
  this->_cachedPollfds.clear();
}

InmemoryClientDatabase::InmemoryClientDatabase(const InmemoryClientDatabase &other) {
  *this = other;
}

InmemoryClientDatabase &InmemoryClientDatabase::operator=(const InmemoryClientDatabase &other) {
  if (this != &other) {
    this->_clients = other._clients;
    this->_cachedPollfds = other._cachedPollfds;
    this->_isPollfdsCached = other._isPollfdsCached;
  }
  return *this;
}

void InmemoryClientDatabase::add(const IClientAggregateRoot &client) {
  this->_clients.push_back(client.clone());
  this->_isPollfdsCached = false;
}

const std::vector<IClientAggregateRoot*> &InmemoryClientDatabase::list() {
  return this->_clients;
}

const std::vector<pollfd> &InmemoryClientDatabase::listPollfds() {
  if (this->_isPollfdsCached) {
    return this->_cachedPollfds;
  }
  this->_cachedPollfds.clear();
  std::vector<IClientAggregateRoot*>::const_iterator it;
  for (it = this->_clients.begin(); it != this->_clients.end(); ++it) {
      this->_cachedPollfds.push_back((*it)->getPollfd());
  }
  this->_isPollfdsCached = true;
  return this->_cachedPollfds;
}

const IClientAggregateRoot &InmemoryClientDatabase::getById(const int id) {
  std::vector<IClientAggregateRoot*>::iterator it;
  for (it = this->_clients.begin(); it != this->_clients.end(); it++) {
    if (id == (*it)->getId()) {
      return *(*it);
    }
  }
  throw std::runtime_error("Not found");
}

void InmemoryClientDatabase::update(const int id, const IClientAggregateRoot &newData) {
  std::vector<IClientAggregateRoot*>::iterator it;
  for (it = this->_clients.begin(); it != this->_clients.end(); it++) {
    if (id == (*it)->getId()) {
      delete *it;
      *it = newData.clone();
      return ;
    }
  }
  this->_isPollfdsCached = false;
  throw std::runtime_error("Not found");
}

void InmemoryClientDatabase::remove(const int id) {
  std::vector<IClientAggregateRoot*>::iterator it;
  for (it = this->_clients.begin(); it != this->_clients.end(); it++) {
    if (id == (*it)->getId()) {
      delete *it;
      this->_clients.erase(it);
      this->_isPollfdsCached = false;
      return ;
    }
  }
  this->_isPollfdsCached = false;
  throw std::runtime_error("Not found");
}
