#include "application/commands/Join.hpp"

typedef std::vector<std::string>::iterator iter;

inline static bool checkChannelName(const std::string &channelName);
inline static void split(const std::string &s, char delim, std::vector<std::string> &elems);
inline static void generateChannelInfoResponse(
    MessageStreamVector &messageStreams,
    ISocketHandler *socketHandler,
    IClientAggregateRoot *client,
    IChannelAggregateRoot *channel);
inline static SendMsgDTO sendError(IClientAggregateRoot *client);

Join::Join(IMessageAggregateRoot *msg, IClientAggregateRoot *client) : ACommands(msg, client) {}

SendMsgDTO Join::execute() {
  MultiLogger *logger = LoggerServiceLocator::get();
  IMessageAggregateRoot *msg = this->getMessage();
  IClientAggregateRoot *client = this->getClient();
  InmemoryChannelDatabase &channelDB = InmemoryChannelDBServiceLocator::get();
  InmemoryClientDatabase &clientDB = InmemoryClientDBServiceLocator::get();
  ISocketHandler *socketHandler = &SocketHandlerServiceLocator::get();

  MessageStreamVector messageStreams;

  const std::string serverName = ConfigsServiceLocator::get().getConfigs().Global.Name;

  if (ClientService::login(*client) != ClientService::LOGIN_ALREADY) {
    MessageStream stream = MessageService::generateMessageStream(socketHandler, client);
    stream << Message(
        serverName, MessageConstants::ResponseCode::ERR_NOTREGISTERED,
        "* :Connection not registered");
    messageStreams.push_back(stream);
    return SendMsgDTO(1, messageStreams);
  }

  if (client->getNickName() == "") {
    return sendError(client);
  }
  if (msg->getParams().size() < 1 || msg->getParams().size() > 2) {
    return sendError(client);
  }
  if (msg->getParams()[0] == "0") {
    const IdToChannelMap &idToChannel = channelDB.getDatabase();
    IdToChannelMap::const_iterator it;
    for (it = idToChannel.begin(); it != idToChannel.end(); ++it) {
      IChannelAggregateRoot *channel = (*it).second;
      if (channel->getListConnects().hasClient(client->getNickName())) {
        channel->getListConnects().removeClient(client->getNickName());
        logger->debugss() << "[PART] by (fd: " << client->getSocketFd() << "): left "
                          << (*it).first;
      }
    }
    return SendMsgDTO(); // TODO: PART message
  }

  std::vector<std::string> channels;
  std::vector<std::string> channelwasswords;
  split(msg->getParams()[0], ',', channels);
  if (msg->getParams().size() == 2) {
    split(msg->getParams()[1], ',', channelwasswords);
    if (channels.size() != channelwasswords.size()) {
      return sendError(client);
    }
  }

  for (size_t i = 0; i < channels.size(); i++) {
    if (!checkChannelName(channels[i])) {
      return sendError(client);
    }

    IChannelAggregateRoot *channel = channelDB.get(channels[i]);
    std::string password = "";
    if (!channelwasswords.empty()) {
      password = channelwasswords[i];
    }
    if (channel == NULL) {
      channelDB.add(Channel(channels[i]));
      channel = channelDB.get(channels[i]);
      channel->addOperator(client->getNickName());
      logger->debugss() << "[JOIN] (fd: " << client->getSocketFd() << "): create " << channels[i];
    } else {
      // パスワードのチェック
      if (!channel->checkKey(password)) {
        sendError(client); // TODO: JOIN messageStreams
        continue;
      }
      // ユーザー数制限のチェック
      if (channel->isMemberLimitExceeded()) {
        sendError(client); // TODO: JOIN messageStreams
        continue;
      }
    }
    int joinResult = channel->getListConnects().addClient(client->getNickName());
    if (joinResult == 1) {
      logger->debugss() << "[JOIN] (fd: " << client->getSocketFd() << "): already joined to "
                        << channels[i];
    } else {
      logger->debugss() << "[JOIN] (fd: " << client->getSocketFd() << "): success to join"
                        << channels[i];
      // チャンネルメンバーにJOIN通知をブロードキャスト
      std::vector<MessageStream> streams;
      std::stringstream joinMsg;
      joinMsg << ":" << client->getNickName() << " JOIN " << channel->getName() << "\r\n";

      streams = MessageService::generateMessageToChannel(
          socketHandler, client, &clientDB, channel, joinMsg.str());
      messageStreams.insert(messageStreams.end(), streams.begin(), streams.end());
      generateChannelInfoResponse(messageStreams, socketHandler, client, channel);
    }
  }
  logger->debugss() << "Total Channels Count: " << channels.size();
  return SendMsgDTO(0, messageStreams);
}

static inline void split(const std::string &s, char delim, std::vector<std::string> &elems) {
  std::stringstream ss(s);
  std::string item;
  while (std::getline(ss, item, delim)) {
    elems.push_back(item);
  }
}

inline static bool checkChannelName(const std::string &channelName) {
  if (channelName.empty() || channelName.length() > 50) {
    return false;
  }
  if (channelName[0] != '#' && channelName[0] != '&') {
    return false;
  }
  const std::string invalidChars = " ,:\r\n";
  if (channelName.find_first_of(invalidChars) != std::string::npos) {
    return false;
  }

  return true;
}

inline static void generateChannelInfoResponse(
    MessageStreamVector &messageStreams,
    ISocketHandler *socketHandler,
    IClientAggregateRoot *client,
    IChannelAggregateRoot *channel) {

  Message response;

  // トピックが設定されている場合は送信
  const std::string serverName = ConfigsServiceLocator::get().getConfigs().Global.Name;
  const std::string &topic = channel->getTopic();

  if (!topic.empty()) {
    response = Message(
        serverName, MessageConstants::ResponseCode::RPL_TOPIC,
        client->getNickName() + " " + channel->getName() + " :" + topic);
    messageStreams.push_back(MessageStream(socketHandler, client) << response);
  }

  const std::vector<std::string> &members = channel->getListConnects().getClients();
  std::string membersStr = "";
  for (std::vector<std::string>::const_iterator member = members.begin(); member != members.end();
       ++member) {
    if (member != members.begin()) {
      membersStr += " ";
    }
    membersStr += *member;
  }
  membersStr += "\r\n";
  response = Message(
      serverName, MessageConstants::ResponseCode::RPL_NAMREPLY,
      client->getNickName() + " " + channel->getName() + " :" + membersStr);
  messageStreams.push_back(MessageStream(socketHandler, client) << response);

  // NAMES終了の通知
  response = Message(
      serverName, MessageConstants::ResponseCode::RPL_ENDOFNAMES,
      client->getNickName() + " " + channel->getName() + " :End of /NAMES list.");
  messageStreams.push_back(MessageStream(socketHandler, client) << response);
}

inline static SendMsgDTO sendError(IClientAggregateRoot *client) {
  const std::string serverName = ConfigsServiceLocator::get().getConfigs().Global.Name;

  MessageStreamVector messageStreams;
  MessageStream stream =
      MessageService::generateMessageStream(&SocketHandlerServiceLocator::get(), client);
  stream << Message(serverName, MessageConstants::ResponseCode::ERR_NEEDMOREPARAMS, "* : Error");
  messageStreams.push_back(stream);
  return SendMsgDTO(1, messageStreams);
}
