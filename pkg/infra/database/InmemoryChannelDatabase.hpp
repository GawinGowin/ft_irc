#ifndef INMEMORYCHANNELDATABASE_HPP
#define INMEMORYCHANNELDATABASE_HPP

#include "domain/channel/IChannelRepository.hpp"

#include <map>
#include <utility>

class InmemoryChannelDatabase : public IChannelRepository {
public:
  InmemoryChannelDatabase();
  ~InmemoryChannelDatabase();
  InmemoryChannelDatabase(const InmemoryChannelDatabase &other);
  InmemoryChannelDatabase &operator=(const InmemoryChannelDatabase &other);

  void add(const IChannelAggregateRoot &channel);
  const std::vector<IChannelAggregateRoot *> &list();
  const IChannelAggregateRoot &get(const ChannelId &id, const std::string &name);
  void update(const ChannelId &id, const IChannelAggregateRoot &newData);
  void remove(const ChannelId &id);

private:
  std::map<std::pair<ChannelId, std::string>, IChannelAggregateRoot *> _channels;
};

#endif /* INMEMORYCHANNELDATABASE_HPP */
