#include "domain/channel/ChannelService.hpp"
#include "infra/database/InmemoryChannelDatabase.hpp"
#include <string>

void ChannelService::purgeClient(const ListofClientList &lsts, const ClientUniqueID &id) {
  ListofClientList::const_iterator it;
  for (it = lsts.begin(); it != lsts.end(); ++it) {
    (*it)->removeClient(id);
  }
}

void ChannelService::removeClientFromAllChannels(const ClientUniqueID &id) {
  InmemoryChannelDatabase &db = InmemoryChannelDBServiceLocator::get();
  const IdToChannelMap &channels = db.getDatabase();

  for (IdToChannelMap::const_iterator it = channels.begin(); it != channels.end(); ++it) {
    IChannelAggregateRoot *channel = it->second;
    ListofClientList lists;
    lists.push_back(&channel->getListConnects());
    lists.push_back(&channel->getListBans());
    lists.push_back(&channel->getListExcepts());
    lists.push_back(&channel->getListInvites());
    purgeClient(lists, id);
  }
}