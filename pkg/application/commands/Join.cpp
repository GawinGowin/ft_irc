#include "application/commands/Join.hpp"

typedef std::vector<std::string>::iterator iter;

static inline bool checkChannelName(const std::string &channelName);
static inline void split(const std::string &s, char delim, std::vector<std::string> &elems);
static inline SendMsgDTO exit_with_error(const std::string &msg, IClientAggregateRoot &client);
static inline std::string
generateChannelInfoResponse(IClientAggregateRoot *client, IChannelAggregateRoot *channel);

Join::Join(IMessageAggregateRoot *msg, IClientAggregateRoot *client) : ACommands(msg, client) {}

SendMsgDTO Join::execute() {
  MultiLogger *logger = LoggerServiceLocator::get();
  IMessageAggregateRoot *msg = this->getMessage();
  IClientAggregateRoot *client = this->getClient();
  InmemoryChannelDatabase &db = InmemoryChannelDBServiceLocator::get();
  InmemoryClientDatabase &clientDB = InmemoryClientDBServiceLocator::get();
  ISocketHandler *socketHandler = &SocketHandlerServiceLocator::get();

  if (client->getNickName() == "") {
    return exit_with_error("You must set your nickname first", *client);
  }
  if (msg->getParams().size() < 1 || msg->getParams().size() > 2) {
    return exit_with_error("Invalid number of parameters", *client);
  }
  if (msg->getParams()[0] == "0") {
    const IdToChannelMap &db = InmemoryChannelDBServiceLocator::get().getDatabase();
    IdToChannelMap::const_iterator it;
    for (it = db.begin(); it != db.end(); ++it) {
      IChannelAggregateRoot *channel = (*it).second;
      if (channel->getListConnects().hasClient(client->getNickName())) {
        channel->getListConnects().removeClient(client->getNickName());
        logger->debugss() << "[PART] by (fd: " << client->getSocketFd() << "): left "
                          << (*it).first;
      }
    }
    return SendMsgDTO();
  }

  std::vector<std::string> channels;
  std::vector<std::string> channelwasswords;
  split(msg->getParams()[0], ',', channels);
  if (msg->getParams().size() == 2) {
    split(msg->getParams()[1], ',', channelwasswords);
    if (channels.size() != channelwasswords.size()) {
      return exit_with_error("Syntax error", *client);
    }
  }

  // 全チャンネルの応答を結合するためのストリーム
  std::stringstream combinedResponse;

  for (size_t i = 0; i < channels.size(); i++) {
    if (!checkChannelName(channels[i])) {
      return exit_with_error("Invalid channel name", *client);
    }

    IChannelAggregateRoot *channel = db.get(channels[i]);
    std::string password = "";
    if (!channelwasswords.empty()) {
      password = channelwasswords[i];
    }

    if (channel == NULL) {
      db.add(Channel(channels[i]));
      channel = db.get(channels[i]);
    } else {
      // パスワードのチェック
      if (!channel->checkKey(password)) {
        exit_with_error("Invalid channel key", *client);
        continue;
      }
      // ユーザー数制限のチェック
      if (channel->isMemberLimitExceeded()) {
        exit_with_error("Channel is full", *client);
        continue;
      }
    }

    // チャンネルへの参加
    int joinResult = channel->getListConnects().addClient(client->getNickName());
    if (joinResult == 1) {
      logger->debugss() << "[JOIN] by (fd: " << client->getSocketFd() << "): already joined to "
                        << channels[i];
    } else {
      logger->debugss() << "[JOIN] by (fd: " << client->getSocketFd() << "): success to "
                        << channels[i];

      // チャンネルメンバーにJOIN通知をブロードキャスト
      std::vector<MessageStream> streams;
      std::stringstream joinMsg;
      joinMsg << ":" << client->getNickName() << " JOIN " << channel->getName() << "\r\n";
      streams = MessageService::generateMessageToChannel(
          socketHandler, client, &clientDB, channel, joinMsg.str());
      combinedResponse << generateChannelInfoResponse(client, channel);
      for (std::vector<MessageStream>::iterator it = streams.begin(); it != streams.end(); ++it) {
        int status = it->send();
        if (status != 0) {
          logger->warningss() << "[JOIN] by (fd: " << client->getSocketFd() << "): failed to send "
                              << channels[i];
        }
      }
    }
  }

  // 応答の生成
  SendMsgDTO dto;
  if (!combinedResponse.str().empty()) {
    dto.setMessage(combinedResponse.str());
  }
  return dto;
}

static inline void split(const std::string &s, char delim, std::vector<std::string> &elems) {
  std::stringstream ss(s);
  std::string item;
  while (std::getline(ss, item, delim)) {
    elems.push_back(item);
  }
}

static inline SendMsgDTO exit_with_error(const std::string &msg, IClientAggregateRoot &client) {
  MultiLogger *logger = LoggerServiceLocator::get();
  SendMsgDTO dto;
  dto.setStatus(1);
  dto.setMessage(msg); // TODO: tmp
  logger->debugss() << "[JOIN] by (fd: " << client.getSocketFd() << "): " << msg;
  return dto;
}

static inline bool checkChannelName(const std::string &channelName) {
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

static inline std::string
generateChannelInfoResponse(IClientAggregateRoot *client, IChannelAggregateRoot *channel) {
  std::stringstream response;

  // トピックが設定されている場合は送信
  const std::string &topic = channel->getTopic();
  if (!topic.empty()) {
    response << ":server 332 " << client->getNickName() << " " << channel->getName() << " :"
             << topic << "\r\n";
  }

  // チャンネルメンバーリストを送信 (NAMES)
  const std::vector<std::string> &members = channel->getListConnects().getClients();
  response << ":server 353 " << client->getNickName() << " = " << channel->getName() << " :";

  for (std::vector<std::string>::const_iterator member = members.begin(); member != members.end();
       ++member) {
    if (member != members.begin()) {
      response << " ";
    }
    response << *member;
  }
  response << "\r\n";

  // NAMES終了の通知
  response << ":server 366 " << client->getNickName() << " " << channel->getName()
           << " :End of /NAMES list.\r\n";
  return response.str();
}
