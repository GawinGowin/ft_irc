#ifndef CHANNELSERVICE_HPP
#define CHANNELSERVICE_HPP

#include "application/serviceLocator/InmemoryChannelDBServiceLocator.hpp"
#include "domain/channel/ChannelClientList.hpp"
#include "domain/channel/IChannelRepository.hpp"
// #include "domain/client/IClientRepository.hpp"

#include <vector>

typedef long ClientUniqueID;
typedef std::vector<ChannelClientList *> ListofClientList;

class ChannelService {
public:
  static void purgeClient(const ListofClientList &lsts, const ClientUniqueID &id);
  static void removeClientFromAllChannels(const ClientUniqueID &id);

private:
  ChannelService();
  ~ChannelService();
  ChannelService(const ChannelService &other);
  ChannelService &operator=(const ChannelService &other);
};

#endif /* CHANNELSERVICE_HPP */
