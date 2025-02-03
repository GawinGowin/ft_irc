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
  JenkinsHash id;
  try {
    id = dynamic_cast<const JenkinsHash &>(channel.getId());
  } catch (const std::exception &e) {
    std::cerr << "ChannelDatabase: add: " << e.what() << std::endl;
  }
  this->_database.insert(std::make_pair(id, channel.clone()));
}

const IChannelAggregateRoot *
InmemoryChannelDatabase::get(const IHashAggregateRoot &id, const std::string &name) {
  JenkinsHash hash;
  try {
    hash = dynamic_cast<const JenkinsHash &>(id);
  } catch (const std::exception &e) {
    std::cerr << "ChannelDatabase: get: " << e.what() << std::endl;
  }
  std::pair<IdToChannelMap::const_iterator, IdToChannelMap::const_iterator> range =
      this->_database.equal_range(hash);
  for (IdToChannelMap::const_iterator it = range.first; it != range.second; ++it) {
    if (it->second->getName() == name) {
      return it->second;
    }
  }
  return NULL;
}

void InmemoryChannelDatabase::update(
    const IHashAggregateRoot &id, const std::string &name, const IChannelAggregateRoot &newData) {
  this->remove(id, name);
  this->add(newData);
}

void InmemoryChannelDatabase::remove(const IHashAggregateRoot &id, const std::string &name) {
  JenkinsHash hash;
  try {
    hash = dynamic_cast<const JenkinsHash &>(id);
  } catch (const std::exception &e) {
    std::cerr << "ChannelDatabase: remove: " << e.what() << std::endl;
  }
  std::pair<IdToChannelMap::iterator, IdToChannelMap::iterator> range =
      this->_database.equal_range(hash);
  for (IdToChannelMap::iterator it = range.first; it != range.second; ++it) {
    if (it->second->getName() == name) {
      delete it->second;
      this->_database.erase(it);
      break;
    }
  }
}

const IdToChannelMap &InmemoryChannelDatabase::getDatabase() { return this->_database; }
