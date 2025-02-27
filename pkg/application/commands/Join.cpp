#include "application/commands/Join.hpp"

typedef std::vector<std::string>::iterator iter;

static inline bool checkChannelName(const std::string &channelName);
static inline void split(const std::string &s, char delim, std::vector<std::string> &elems);
static inline SendMsgDTO exit_with_error(const std::string &msg, IClientAggregateRoot &client);

Join::Join(IMessageAggregateRoot *msg, IClientAggregateRoot *client) : ACommands(msg, client) {}

SendMsgDTO Join::execute() {
  MultiLogger *logger = LoggerServiceLocator::get();
  IMessageAggregateRoot *msg = this->getMessage();
  IClientAggregateRoot *client = this->getClient();
  InmemoryChannelDatabase &db = InmemoryChannelDBServiceLocator::get();

  if (client->getNickName() == "") {
    return exit_with_error("You must set your nickname first", *client);
  }
  if (msg->getParams().size() < 1 || msg->getParams().size() > 2) {
    return exit_with_error("Invalid number of parameters", *client);
  }
  if (msg->getParams()[0] == "0") {
    std::vector<std::string> joinedChannels;
    auto db = InmemoryChannelDBServiceLocator::get().getDatabase();
    for (const auto &channelPair : db) {
      IChannelAggregateRoot *channel = channelPair.second;
      if (channel->getListConnects().hasClient(client->getNickName())) {
        channel->getListConnects().removeClient(client->getNickName());
        joinedChannels.push_back(channelPair.first);
        logger->debugss() << "[PART] by (fd: " << client->getSocketFd() << "): left "
                          << channelPair.first;
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
  for (size_t i = 0; i < channels.size(); i++) {
    if (!checkChannelName(channels[i])) {
      return exit_with_error("Invalid channel name", *client);
    }

    IChannelAggregateRoot *channel = db.get(channels[i]);
    std::string password = "";
    if (!channelwasswords.empty()) {
      password = channelwasswords[i];
    }

    if (channel != NULL) {
      // パスワードが設定されているチャンネルの場合
      if (!channel->getKey().empty() && channel->getKey() != password) {
        return exit_with_error("Invalid channel key", *client);
      }

      // ユーザー数制限のチェック
      if (channel->getMaxUsers() > 0 &&
          channel->getListConnects().getClients().size() >= channel->getMaxUsers()) {
        return exit_with_error("Channel is full", *client);
      }
    } else {
      db.add(Channel(channels[i]));
      channel = db.get(channels[i]);
    }

    int joinResult = channel->getListConnects().addClient(client->getNickName());
    if (joinResult == 1) {
      logger->debugss() << "[JOIN] by (fd: " << client->getSocketFd() << "): already joined to "
                        << channels[i];
    } else {
      logger->debugss() << "[JOIN] by (fd: " << client->getSocketFd() << "): success to "
                        << channels[i];

      // JOIN成功メッセージの構築
      std::stringstream joinResponse;
      joinResponse << ":" << client->getNickName() << " JOIN " << channels[i] << "\r\n";

      // トピックが設定されている場合は送信
      if (!channel->getTopic().empty()) {
        joinResponse << ":server 332 " << client->getNickName() << " " << channels[i] << " :"
                     << channel->getTopic() << "\r\n";
      }

      // チャンネルメンバーリストの送信
      const auto &members = channel->getListConnects().getClients();
      joinResponse << ":server 353 " << client->getNickName() << " = " << channels[i] << " :";
      for (const auto &member : members) {
        joinResponse << member << " ";
      }
      joinResponse << "\r\n";
      joinResponse << ":server 366 " << client->getNickName() << " " << channels[i]
                   << " :End of /NAMES list.\r\n";

      SendMsgDTO dto;
      dto.setMessage(joinResponse.str());
      return dto;
    }
  }
  (void)channelwasswords;
  return SendMsgDTO();
}

// static inline int checkJoinChannel();

// static inline int JoinChannel();

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

  // チャンネル名は'#'または'&'で始まる必要がある
  if (channelName[0] != '#' && channelName[0] != '&') {
    return false;
  }

  // 禁止文字のチェック（スペース、カンマ、コロン、CR、LF等）
  const std::string invalidChars = " ,:\r\n";
  if (channelName.find_first_of(invalidChars) != std::string::npos) {
    return false;
  }

  return true;
}
