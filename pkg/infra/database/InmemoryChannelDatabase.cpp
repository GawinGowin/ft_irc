#include "infra/database/InmemoryChannelDatabase.hpp"

InmemoryChannelDatabase::InmemoryChannelDatabase()
    : _channels(std::map<std::pair<ChannelId, std::string>, IChannelAggregateRoot *>()) {}

InmemoryChannelDatabase::~InmemoryChannelDatabase() {}

InmemoryChannelDatabase::InmemoryChannelDatabase(const InmemoryChannelDatabase &other) {
  *this = other;
}

InmemoryChannelDatabase &InmemoryChannelDatabase::operator=(const InmemoryChannelDatabase &other) {
  if (this != &other) {
    this->_channels = other._channels;
  }
  return *this;
}

void InmemoryChannelDatabase::add(const IChannelAggregateRoot &channel) {
  std::pair<ChannelId, std::string> key = std::make_pair(channel.getId(), channel.getName());
  _channels.insert(std::make_pair(key, channel.clone()));
}

const std::vector<IChannelAggregateRoot *> &InmemoryChannelDatabase::list() {
  std::vector<IChannelAggregateRoot *> ret;
  std::map<std::pair<ChannelId, std::string>, IChannelAggregateRoot *>::iterator it;
  for (it = _channels.begin(); it != _channels.end(); ++it) {
    ret.push_back((*it).second);
  }
  return ret;
}

const IChannelAggregateRoot *
InmemoryChannelDatabase::get(const ChannelId &id, const std::string &name) {
  std::pair<ChannelId, std::string> key = std::make_pair(id, name);
  if (_channels.find(key) == _channels.end()) {
    return NULL;
  }
  return _channels.at(key);
}

void InmemoryChannelDatabase::update(
    const ChannelId &id, const std::string &name, const IChannelAggregateRoot &newData) {
  const std::pair<ChannelId, std::string> key = std::make_pair(id, name);
  this->remove(id, name);
  _channels.insert(std::make_pair(key, newData.clone()));
}

void InmemoryChannelDatabase::remove(const ChannelId &id, const std::string &name) {
  const std::pair<ChannelId, std::string> key = std::make_pair(id, name);
  delete _channels.at(key);
  _channels.erase(key);
}
