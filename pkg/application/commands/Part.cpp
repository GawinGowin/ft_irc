#include "application/commands/Part.hpp"

typedef std::vector<std::string>::iterator iter;

inline static void split(const std::string &s, char delim, std::vector<std::string> &elems);

Part::Part(IMessageAggregateRoot *msg, IClientAggregateRoot *client) : ACommands(msg, client) {}

SendMsgDTO Part::execute() {
  MultiLogger *logger = LoggerServiceLocator::get();
  IMessageAggregateRoot *msg = this->getMessage();
  IClientAggregateRoot *client = this->getClient();
  InmemoryChannelDatabase &channelDB = InmemoryChannelDBServiceLocator::get();
  InmemoryClientDatabase &clientDB = InmemoryClientDBServiceLocator::get();
  ISocketHandler *socketHandler = &SocketHandlerServiceLocator::get();

  MessageStreamVector messageStreams;

  const std::string serverName = ConfigsServiceLocator::get().getConfigs().Global.Name;

  if (client->getClientType() != CLIENT_USER) {
    MessageStream stream = MessageService::generateMessageStream(socketHandler, client);
    stream << Message(
        serverName, MessageConstants::ResponseCode::ERR_NOTREGISTERED,
        "* :Connection not registered");
    messageStreams.push_back(stream);
    return SendMsgDTO(1, messageStreams);
  }

  if (msg->getParams().size() < 1) {
    MessageStream stream = MessageService::generateMessageStream(socketHandler, client);
    stream << Message(
        serverName, MessageConstants::ResponseCode::ERR_NEEDMOREPARAMS,
        client->getNickName() + " PART" + " :Syntax error");
    messageStreams.push_back(stream);
    return SendMsgDTO(1, messageStreams);
  }

  // チャンネルリストの取得
  std::vector<std::string> channels;
  split(msg->getParams()[0], ',', channels);

  std::string partMessage;
  if (msg->getParams().size() > 1) {
    partMessage = msg->getParams()[1];
  } else {
    partMessage = client->getNickName();
  }

  for (size_t i = 0; i < channels.size(); i++) {
    IChannelAggregateRoot *channel = channelDB.get(channels[i]);
    
    // チャンネルが存在するか確認
    if (channel == NULL) {
      MessageStream stream = MessageService::generateMessageStream(socketHandler, client);
      stream << Message(
          serverName, MessageConstants::ResponseCode::ERR_NOSUCHCHANNEL,
          client->getNickName() + " " + channels[i] + " :No such channel");
      messageStreams.push_back(stream);
      continue;
    }
    // クライアントがそのチャンネルに参加しているか
    if (!channel->getListConnects().hasClient(client->getNickName())) {
      MessageStream stream = MessageService::generateMessageStream(socketHandler, client);
      stream << Message(
          serverName, MessageConstants::ResponseCode::ERR_NOTONCHANNEL,
          client->getNickName() + " " + channels[i] + " :You're not on that channel");
      messageStreams.push_back(stream);
      continue;
    }

    Message partMsg(
        client->getNickName() + "!" + client->getUserName() + "@" + client->getAddress(),
        MessageConstants::PART,
        channels[i] + " :" + partMessage);
    
    MessageStream stream = MessageService::generateMessageStream(socketHandler, client);
    stream << partMsg;
    messageStreams.push_back(stream);
    
    std::stringstream ss;
    ss << partMsg;
    
    MessageStreamVector channelStreams = MessageService::generateMessageToChannel(
        socketHandler, client, &clientDB, channel, ss.str());
    messageStreams.insert(messageStreams.end(), channelStreams.begin(), channelStreams.end());
    
    channel->getListConnects().removeClient(client->getNickName());
    
    logger->debugss() << "[PART] (fd: " << client->getSocketFd() << "): left " << channels[i];
  }
  
  return SendMsgDTO(0, messageStreams);
}

static inline void split(const std::string &s, char delim, std::vector<std::string> &elems) {
  std::stringstream ss(s);
  std::string item;
  while (std::getline(ss, item, delim)) {
    if (item.empty()) {
      continue;
    }
    elems.push_back(item);
  }
}