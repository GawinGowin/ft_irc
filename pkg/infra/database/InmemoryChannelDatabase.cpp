#include "infra/database/InmemoryChannelDatabase.hpp"

InmemoryChannelDatabase::InmemoryChannelDatabase() : _database(IdToChannelMap()) {}

InmemoryChannelDatabase::~InmemoryChannelDatabase() {
  IdToChannelMap::iterator it;
  for (it = _database.begin(); it != _database.end(); ++it) {
    if ((*it).second != NULL) {
      delete (*it).second;
      (*it).second = NULL;
    }
  }
  _database.clear();
}

InmemoryChannelDatabase::InmemoryChannelDatabase(const InmemoryChannelDatabase &other) {
  *this = other;
}

InmemoryChannelDatabase &InmemoryChannelDatabase::operator=(const InmemoryChannelDatabase &other) {
  if (this != &other) {
    this->_database = other._database;
  }
  return *this;
}

void InmemoryChannelDatabase::add(const IChannelAggregateRoot &channel) {
  this->_database.insert(std::make_pair(channel.getName(), channel.clone()));
}

const IChannelAggregateRoot *InmemoryChannelDatabase::get(const std::string &name) {
  IdToChannelMap::iterator ret = this->_database.find(name);
  return ret == this->_database.end() ? NULL : ret->second;
}

void InmemoryChannelDatabase::update(
    const std::string &name, const IChannelAggregateRoot &newData) {
  this->remove(name);
  this->add(newData);
}

void InmemoryChannelDatabase::remove(const std::string &name) {
  IdToChannelMap::iterator ret = this->_database.find(name);
  if (ret != this->_database.end()) {
    delete ret->second;
    this->_database.erase(ret);
  }
}

const IdToChannelMap &InmemoryChannelDatabase::getDatabase() { return this->_database; }
