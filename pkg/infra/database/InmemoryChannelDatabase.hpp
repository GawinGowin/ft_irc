#ifndef INMEMORYCHANNELDATABASE_HPP
#define INMEMORYCHANNELDATABASE_HPP

#include "domain/channel/IChannelAggregateRoot.hpp"
#include "domain/channel/IChannelRepository.hpp"
#include "domain/shared/values/JenkinsHash.hpp"

#include <iostream>
#include <map>
#include <utility>

typedef std::multimap<JenkinsHash, IChannelAggregateRoot *> IdToChannelMap;

class InmemoryChannelDatabase : public IChannelRepository {
public:
  InmemoryChannelDatabase();
  ~InmemoryChannelDatabase();
  InmemoryChannelDatabase(const InmemoryChannelDatabase &other);
  InmemoryChannelDatabase &operator=(const InmemoryChannelDatabase &other);

  void add(const IChannelAggregateRoot &channel);
  const IChannelAggregateRoot *get(const IHashAggregateRoot &id, const std::string &name);
  void update(
      const IHashAggregateRoot &id, const std::string &name, const IChannelAggregateRoot &newData);
  void remove(const IHashAggregateRoot &id, const std::string &name);

  const IdToChannelMap &getDatabase();

private:
  IdToChannelMap _database;
};

#endif /* INMEMORYCHANNELDATABASE_HPP */
