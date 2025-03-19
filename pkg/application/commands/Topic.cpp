#include "application/commands/Topic.hpp"
#include "application/dto/SendMsgDTO.hpp"
#include "application/serviceLocator/SocketHandlerServiceLocator.hpp"
#include "domain/channel/ChannelTopic.hpp"
#include "domain/message/MessageStreamVector.hpp"
#include "infra/database/InmemoryChannelDatabase.hpp"
#include "infra/socket/ISocketHandler.hpp"

Topic::Topic(IMessageAggregateRoot *msg, IClientAggregateRoot *client) : ACommands(msg, client) {}

SendMsgDTO Topic::execute() {
  std::string channelName = getMessage()->getParams()[0];

  // チャンネルの存在確認
  InmemoryChannelDatabase &db = InmemoryChannelDBServiceLocator::get();
  IChannelAggregateRoot *channel = db.get(channelName);
  if (channel == NULL) {
    MessageStreamVector msgStreams;
    SocketHandler &socketHandler = SocketHandlerServiceLocator::get();
    MessageStream stream(&socketHandler, getClient());
    stream << "403 " << getClient()->getNickName() << " " << channelName << " :No such channel\r\n";
    msgStreams.push_back(stream);
    return SendMsgDTO(403, msgStreams);
  }

  // クライアントがチャンネルに参加しているか確認
  if (!channel->getListConnects().hasClient(getClient()->getNickName())) {
    MessageStreamVector msgStreams;
    SocketHandler &socketHandler = SocketHandlerServiceLocator::get();
    MessageStream stream(&socketHandler, getClient());
    stream << "442 " << getClient()->getNickName() << " " << channelName
           << " :You're not on that channel\r\n";
    msgStreams.push_back(stream);
    return SendMsgDTO(442, msgStreams);
  }

  // トピックの設定または表示
  if (getMessage()->getParams().size() > 1) {
    // トピック設定
    std::string topic = getMessage()->getParams()[1];
    return setTopic(channel, topic);
  } else {
    // トピック表示
    return showTopic(channel);
  }
}

SendMsgDTO Topic::showTopic(IChannelAggregateRoot *channel) {
  const std::string &topic = channel->getTopic();
  MessageStreamVector msgStreams;
  SocketHandler &socketHandler = SocketHandlerServiceLocator::get();
  MessageStream stream(&socketHandler, getClient());

  if (topic.empty()) {
    stream << "331 " << getClient()->getNickName() << " " << channel->getName()
           << " :No topic is set\r\n";
    msgStreams.push_back(stream);
    return SendMsgDTO(331, msgStreams);
  }

  stream << "332 " << getClient()->getNickName() << " " << channel->getName() << " :" << topic
         << "\r\n";
  msgStreams.push_back(stream);
  return SendMsgDTO(332, msgStreams);
}

SendMsgDTO Topic::setTopic(IChannelAggregateRoot *channel, const std::string &topic) {
  // TODO: トピック設定の権限チェックを実装する必要があります

  // トピックを更新
  channel->updateTopic(topic, getClient()->getNickName());

  // 応答メッセージの作成
  MessageStreamVector msgStreams;
  SocketHandler &socketHandler = SocketHandlerServiceLocator::get();
  MessageStream stream(&socketHandler, getClient());
  stream << ":" << getClient()->getNickName() << " TOPIC " << channel->getName() << " :" << topic
         << "\r\n";
  msgStreams.push_back(stream);
  return SendMsgDTO(0, msgStreams);
}
