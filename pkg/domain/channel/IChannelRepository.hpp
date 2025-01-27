#ifndef ICHANNELREPOSITORY_HPP
#define ICHANNELREPOSITORY_HPP

#include "domain/channel/ChannelId.hpp"
#include "domain/channel/IChannelAggregateRoot.hpp"

class IChannelRepository {
public:
  virtual ~IChannelRepository(){};

  virtual void add(const IChannelAggregateRoot &IChannelAggregateRoot) = 0;
  virtual const std::vector<IChannelAggregateRoot *> &list() = 0;
  virtual const IChannelAggregateRoot &get(const ChannelId &id, const std::string &name) = 0;
  virtual void update(const ChannelId &id, const IChannelAggregateRoot &newData) = 0;
  virtual void remove(const ChannelId &id) = 0;
};

#endif /* ICHANNELREPOSITORY_HPP */
