#include "application/commands/Mode.hpp"

Mode::Mode(IMessageAggregateRoot *msg, IClientAggregateRoot *client) : ACommands(msg, client) {
  this->_channelDB = &InmemoryChannelDBServiceLocator::get();
  this->_clientDB = &InmemoryClientDBServiceLocator::get();
  this->_logger = LoggerServiceLocator::get();
  this->_conf = &ConfigsServiceLocator::get();
  this->_socketHandler = &SocketHandlerServiceLocator::get();
}

SendMsgDTO Mode::execute() {
  IMessageAggregateRoot *msg = this->getMessage();
  IClientAggregateRoot *client = this->getClient();

  const Configs &configs = this->_conf->getConfigs();
  if (msg->getParams().size() < 1) {
    MessageStreamVector streams;
    MessageStream stream = MessageService::generateMessageStream(this->_socketHandler, client);
    stream << Message(
        configs.Global.Name, MessageConstants::ResponseCode::ERR_NEEDMOREPARAMS,
        client->getNickName() + " MODE" + " :Syntax error");
    streams.push_back(stream);
    this->_logger->debugss() << "[MODE]: err syndax (" << client->getSocketFd() << ")";
    return SendMsgDTO(1, streams);
  }
  std::string target = msg->getParams()[0];
  if (configs.Options.AllowedChannelTypes.find(target[0]) !=
      std::string::npos) { // 引数が"#"もしくは"&"で始まる場合
    return this->_handleChannelMode(msg, client);
  }
  return this->_handleUserMode(msg, client);
}

SendMsgDTO Mode::_handleChannelMode(IMessageAggregateRoot *msg, IClientAggregateRoot *client) {
  MessageStreamVector streams;

  const std::string &channelName = msg->getParams()[0];
  IChannelAggregateRoot *channel = this->_channelDB->get(channelName);
  if (!channel) {
    MessageStream stream = MessageService::generateMessageStream(this->_socketHandler, client);
    stream << Message(
        ConfigsServiceLocator::get().getConfigs().Global.Name,
        MessageConstants::ResponseCode::ERR_NOSUCHNICK,
        client->getNickName() + " " + channelName + " :No such nick or channel name");
    streams.push_back(stream);
    this->_logger->debugss() << "[MODE]: no such channel (" << client->getSocketFd() << ")";
    return SendMsgDTO(1, streams);
  }
  // チャンネルのモードを表示
  if (msg->getParams().size() == 1) {
    std::string modeString = "+";
    std::string modeParams = "";
    int modeFlags = channel->getModeFlags();
    if (modeFlags & IChannelAggregateRoot::MODE_INVITE_ONLY) {
      modeString += "i";
    }
    if (modeFlags & IChannelAggregateRoot::MODE_TOPIC_RESTRICTED) {
      modeString += "t";
    }
    if (modeFlags & IChannelAggregateRoot::MODE_KEY_PROTECTED) {
      modeString += "k";
      modeParams += " " + channel->getKey();
    }
    if (modeFlags & IChannelAggregateRoot::MODE_LIMIT_USERS) {
      modeString += "l";
      modeParams += " " + channel->getMaxUsers();
    }
    std::stringstream ss;
    ss << channel->getCreationTime();
    std::string creationTimeStr = ss.str();

    Message modeResponse = Message(
        ConfigsServiceLocator::get().getConfigs().Global.Name,
        MessageConstants::ResponseCode::RPL_CHANNELMODEIS,
        client->getNickName() + " " + channelName + " " + modeString + " " + modeParams);
    Message creationtimeResponse = Message(
        ConfigsServiceLocator::get().getConfigs().Global.Name,
        MessageConstants::ResponseCode::RPL_CREATIONTIME_MSG,
        client->getNickName() + " " + channelName + " " + creationTimeStr);
    streams.push_back(
        MessageStream(this->_socketHandler, client) << modeResponse << creationtimeResponse);
    this->_logger->debugss() << "[MODE]: show mode (" << client->getSocketFd() << ")";
    return SendMsgDTO(0, streams);
  }
  if (!channel->isOperator(client->getNickName())) {
    MessageStream stream = MessageService::generateMessageStream(this->_socketHandler, client);
    stream << Message(
        ConfigsServiceLocator::get().getConfigs().Global.Name,
        MessageConstants::ResponseCode::ERR_CHANOPRIVSNEEDED,
        client->getNickName() + " " + channelName + " :You are not channel operator");
    streams.push_back(stream);
    this->_logger->debugss() << "[MODE]: not channel operator (" << client->getSocketFd() << ")";
    return SendMsgDTO(1, streams);
  }
  // チャンネルのモードを変更
  // MODE <channel> [{+|-}<mode>[<mode>] [<arg> [<arg> [...]]] [{+|-}<mode>[<mode>] [<arg> [<arg> [...]]] [...]]]
  ModeChanges actualModeModifise;
  int err = this->_parseAndProcessChannelMode(&actualModeModifise, channel, msg->getParams());
  if (err != 0) {
    MessageStream stream = MessageService::generateMessageStream(this->_socketHandler, client);
    switch (err) {
    case MessageConstants::ResponseCode::ERR_NOSUCHNICK:
      stream << Message(
          ConfigsServiceLocator::get().getConfigs().Global.Name,
          MessageConstants::ResponseCode::ERR_NOSUCHNICK,
          client->getNickName() + " " + channelName + " :No such nick or channel name");
      break;
    case MessageConstants::ResponseCode::ERR_USERNOTINCHANNEL:
      stream << Message(
          ConfigsServiceLocator::get().getConfigs().Global.Name,
          MessageConstants::ResponseCode::ERR_USERNOTINCHANNEL,
          client->getNickName() + " " + channelName + " :They aren't on that channel");
      break;
    case MessageConstants::ResponseCode::ERR_UNKNOWNMODE:
      stream << Message(
          ConfigsServiceLocator::get().getConfigs().Global.Name,
          MessageConstants::ResponseCode::ERR_UNKNOWNMODE,
          client->getNickName() + " " + channelName + " :Unknown mode");
      break;
    case MessageConstants::ResponseCode::ERR_INVALIDMODEPARAM:
      stream << Message(
          ConfigsServiceLocator::get().getConfigs().Global.Name,
          MessageConstants::ResponseCode::ERR_INVALIDMODEPARAM,
          client->getNickName() + " " + channelName + " :Invalid mode parameter");
      break;
    default:
      break;
    }
    streams.push_back(stream);
    this->_logger->debugss() << "[MODE]: invalid mode parameter (" << client->getSocketFd() << ")";
    return SendMsgDTO(1, streams);
  }
  /* TODO: 下記を修正 */
  std::string modeChangesResponse = "";
  std::stringstream ss;
  ss << Message(
      client->getNickName() + "!" + client->getUserName() + "@" + client->getAddress(),
      MessageConstants::MODE, channelName + " " + modeChangesResponse);
  InmemoryClientDatabase &clientDB = InmemoryClientDBServiceLocator::get();
  MessageStreamVector newStreams = MessageService::generateMessageToChannel(
      this->_socketHandler, client, &clientDB, channel, ss.str());
  streams.insert(streams.end(), newStreams.begin(), newStreams.end());
  MessageStream stream = MessageService::generateMessageStream(this->_socketHandler, client);
  streams.push_back(stream << ss.str());
  this->_logger->debugss() << "[MODE]: change mode (" << client->getSocketFd() << ")";
  return SendMsgDTO(0, streams);
}

int Mode::_check_invalid_nick_arg(std::string targetNick, IChannelAggregateRoot *channel) {
  if (this->_clientDB->getById(targetNick) == NULL)
    return (MessageConstants::ResponseCode::ERR_NOSUCHNICK);
  if (!channel->getListConnects().isClientInList(targetNick))
    return (MessageConstants::ResponseCode::ERR_USERNOTINCHANNEL);
  return (0);
}

inline static int is_valid_limits_arg(std::string limitStr, int *value) {
  int tmp_value;
  std::stringstream ss(limitStr);
  ss >> tmp_value;
  if (ss.fail() || !ss.eof()) {
    return MessageConstants::ResponseCode::ERR_INVALIDMODEPARAM;
  }
  if (tmp_value <= 0 || tmp_value >= 65535) {
    return MessageConstants::ResponseCode::ERR_INVALIDMODEPARAM;
  }
  *value = tmp_value;
  return 0;
}

int Mode::_parseAndProcessChannelMode(
    ModeChanges *actualModeModifise,
    IChannelAggregateRoot *channel,
    const std::vector<std::string> &modeParams) {
  int modeFlags = channel->getModeFlags();
  actualModeModifise->modeFlags = modeFlags;
  std::vector<std::string>::const_iterator it;
  for (it = modeParams.begin(); it != modeParams.end(); ++it) {
    bool isAdd = true;
    if (it->length() == 1 && it->at(0) == '+') {
      isAdd = true;
      continue;
    } else if (it->length() == 1 && it->at(0) == '-') {
      isAdd = false;
      continue;
    }
    size_t token_len = it->length();
    for (size_t i = 0; i < token_len; i++) {
      char c = it->at(i);
      switch (c) {
      case '+':
        isAdd = true;
        break;
      case '-':
        isAdd = false;
        break;
      case 'i': // invite-only
        if (isAdd) {
          actualModeModifise->modeFlags |= IChannelAggregateRoot::MODE_INVITE_ONLY;
        } else {
          actualModeModifise->modeFlags &= ~IChannelAggregateRoot::MODE_INVITE_ONLY;
        }
        break;
      case 't': // topic制限
        if (isAdd) {
          actualModeModifise->modeFlags |= IChannelAggregateRoot::MODE_TOPIC_RESTRICTED;
        } else {
          actualModeModifise->modeFlags &= ~IChannelAggregateRoot::MODE_TOPIC_RESTRICTED;
        }
        break;
      case 'k': // キー（パスワード）
        if (isAdd && it + 1 != modeParams.end()) {
          const std::string &key = *(++it);
          actualModeModifise->modeFlags |= IChannelAggregateRoot::MODE_KEY_PROTECTED;
          actualModeModifise->newChannelKey = key;
        } else {
          actualModeModifise->modeFlags &= ~IChannelAggregateRoot::MODE_KEY_PROTECTED;
          actualModeModifise->newChannelKey = "";
        }
        break;
      case 'o': // オペレータ権限の付与/剥奪
        if (it + 1 != modeParams.end()) {
          const std::string &targetNick = *(++it);
          int ret = this->_check_invalid_nick_arg(targetNick, channel);
          if (ret != 0) {
            return ret;
          }
          if (isAdd) {
            actualModeModifise->newOperators.insert(targetNick);
            actualModeModifise->removedOperators.erase(targetNick);
          } else {
            actualModeModifise->newOperators.erase(targetNick);
            actualModeModifise->removedOperators.insert(targetNick);
          }
        }
        break;
      case 'l': // ユーザー数制限
        if (isAdd) {
          if (it + 1 != modeParams.end()) {
            const std::string &limitStr = *(++it);
            int limitValue;
            int ret = is_valid_limits_arg(limitStr, &limitValue);
            if (ret != 0) {
              return ret;
            }
            actualModeModifise->modeFlags |= IChannelAggregateRoot::MODE_LIMIT_USERS;
            actualModeModifise->newChannelLimit = limitValue;
          }
        } else {
          actualModeModifise->modeFlags &= ~IChannelAggregateRoot::MODE_LIMIT_USERS;
          actualModeModifise->newChannelLimit = 0;
        }
        break;
      default:
        return MessageConstants::ResponseCode::ERR_UNKNOWNMODE;
        break;
      }
    }
  }
  return 0;
}

SendMsgDTO Mode::_handleUserMode(IMessageAggregateRoot *msg, IClientAggregateRoot *client) {
  MessageStreamVector streams;
  (void)msg;
  (void)client;
  // ユーザーモード処理の実装
  return SendMsgDTO(0, streams);
}
