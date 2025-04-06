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
  MessageStreamVector streams;
  ISocketHandler *socketHandler = &SocketHandlerServiceLocator::get();
  const std::string serverName = ConfigsServiceLocator::get().getConfigs().Global.Name;

  if (client->getClientType() != CLIENT_USER) {
    MessageStream stream = MessageService::generateMessageStream(socketHandler, client);
    stream << Message(
        serverName, MessageConstants::ResponseCode::ERR_NOTREGISTERED,
        "* :Connection not registered");
    streams.push_back(stream);
    return SendMsgDTO(1, streams);
  }

  if (msg->getParams().size() < 1) {
    MessageStream stream = MessageService::generateMessageStream(this->_socketHandler, client);
    stream << Message(
        configs.Global.Name, MessageConstants::ResponseCode::ERR_NEEDMOREPARAMS,
        client->getNickName() + " MODE" + " :Syntax error");
    streams.push_back(stream);
    this->_logger->debugss() << "[MODE]: err syndax (fd:" << client->getSocketFd() << ")";
    return SendMsgDTO(1, streams);
  }
  std::string target = msg->getParams()[0];
  // 引数が"#"もしくは"&"で始まる場合
  if (configs.Options.AllowedChannelTypes.find(target[0]) != std::string::npos) {
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
    this->_logger->debugss() << "[MODE]: no such channel (fd:" << client->getSocketFd() << ")";
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
    this->_logger->debugss() << "[MODE]: show mode (fd:" << client->getSocketFd() << ")";
    return SendMsgDTO(0, streams);
  }
  if (!channel->isOperator(client->getNickName())) {
    MessageStream stream = MessageService::generateMessageStream(this->_socketHandler, client);
    stream << Message(
        ConfigsServiceLocator::get().getConfigs().Global.Name,
        MessageConstants::ResponseCode::ERR_CHANOPRIVSNEEDED,
        client->getNickName() + " " + channelName + " :You are not channel operator");
    streams.push_back(stream);
    this->_logger->debugss() << "[MODE]: not channel operator (fd:" << client->getSocketFd() << ")";
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
    this->_logger->debugss() << "[MODE]: invalid parameter (fd:" << client->getSocketFd() << ")";
  }

  const int &newFlags = actualModeModifise.modeFlags;
  if (newFlags & IChannelAggregateRoot::MODE_KEY_PROTECTED) {
    channel->setKey(actualModeModifise.newChannelKey);
  } else if (!(newFlags & IChannelAggregateRoot::MODE_KEY_PROTECTED)) {
    channel->setKey("");
  }
  if (newFlags & IChannelAggregateRoot::MODE_LIMIT_USERS) {
    channel->setMaxUsers(actualModeModifise.newChannelLimit);
  } else if (!(newFlags & IChannelAggregateRoot::MODE_LIMIT_USERS)) {
    channel->setMaxUsers(0); // TODO: set default value
  }
  for (std::set<std::string>::const_iterator it = actualModeModifise.newOperators.begin();
       it != actualModeModifise.newOperators.end(); ++it) {
    channel->addOperator(*it);
  }
  for (std::set<std::string>::const_iterator it = actualModeModifise.removedOperators.begin();
       it != actualModeModifise.removedOperators.end(); ++it) {
    channel->removeOperator(*it);
  }

  channel->setModeFlags(newFlags);

  const std::string modeChangesResponse =
      actualModeModifise.ChangedFlags + " " + actualModeModifise.ChangedParams;
  // メッセージを生成して送信
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
    ModeChanges *mod, IChannelAggregateRoot *channel, const std::vector<std::string> &modeArgs) {
  mod->modeFlags = channel->getModeFlags();
  mod->newChannelKey = channel->getKey();

  // 0 <= getMaxUsers <= 65534
  mod->newChannelLimit = std::atoi(channel->getMaxUsers().c_str());

  mod->ChangedFlags = "";
  mod->ChangedParams = "";
  std::vector<std::string>::const_iterator it;
  bool isAdd = true;
  for (it = modeArgs.begin() + 1; it != modeArgs.end(); ++it) {
    if (it->length() == 1 && it->at(0) == '+' && !isAdd) {
      if (mod->ChangedFlags.empty())
        mod->ChangedFlags += "+";
      isAdd = true;
      continue;
    } else if (it->length() == 1 && it->at(0) == '-' && isAdd) {
      if (mod->ChangedFlags.empty())
        mod->ChangedFlags += "-";
      isAdd = false;
      continue;
    }
    size_t token_len = it->length();
    for (size_t i = 0; i < token_len; i++) {
      char c = it->at(i);
      switch (c) {
      case '+':
        if (!isAdd || mod->ChangedFlags.empty())
          mod->ChangedFlags += "+";
        isAdd = true;
        continue;
      case '-':
        if (isAdd || mod->ChangedFlags.empty())
          mod->ChangedFlags += "-";
        isAdd = false;
        continue;
      case 'i': // invite-only
        if (isAdd && !(mod->modeFlags & IChannelAggregateRoot::MODE_INVITE_ONLY)) {
          mod->modeFlags |= IChannelAggregateRoot::MODE_INVITE_ONLY;
          mod->ChangedFlags += "i";
        } else if (!isAdd && (mod->modeFlags & IChannelAggregateRoot::MODE_INVITE_ONLY)) {
          mod->modeFlags &= ~IChannelAggregateRoot::MODE_INVITE_ONLY;
          mod->ChangedFlags += "i";
        }
        break;
      case 't': // topic制限
        if (isAdd && !(mod->modeFlags & IChannelAggregateRoot::MODE_TOPIC_RESTRICTED)) {
          mod->modeFlags |= IChannelAggregateRoot::MODE_TOPIC_RESTRICTED;
          mod->ChangedFlags += "t";
        } else if (!isAdd && (mod->modeFlags & IChannelAggregateRoot::MODE_TOPIC_RESTRICTED)) {
          mod->modeFlags &= ~IChannelAggregateRoot::MODE_TOPIC_RESTRICTED;
          mod->ChangedFlags += "t";
        }
        break;
      case 'k': // キー（パスワード）
        if (isAdd && it + 1 != modeArgs.end()) {
          const std::string &key = *(++it);
          mod->modeFlags |= IChannelAggregateRoot::MODE_KEY_PROTECTED;
          mod->newChannelKey = key;
          mod->ChangedFlags += "k";
          mod->ChangedParams += " " + key;
        } else if (!isAdd) {
          mod->modeFlags &= ~IChannelAggregateRoot::MODE_KEY_PROTECTED;
          mod->newChannelKey = "";
          mod->ChangedFlags += "k";
        } else {
          return MessageConstants::ResponseCode::ERR_INVALIDMODEPARAM;
        }
        break;
      case 'o': // オペレータ権限の付与/剥奪
        if (it + 1 != modeArgs.end()) {
          const std::string &targetNick = *(++it);
          int ret = this->_check_invalid_nick_arg(targetNick, channel);
          if (ret != 0) {
            return ret;
          }
          if (isAdd && mod->newOperators.find(targetNick) == mod->newOperators.end()) {
            mod->newOperators.insert(targetNick);
            mod->removedOperators.erase(targetNick);
            mod->ChangedFlags += "o";
            mod->ChangedParams += " " + targetNick;

          } else if (
              !isAdd && mod->removedOperators.find(targetNick) != mod->removedOperators.end()) {
            mod->newOperators.erase(targetNick);
            mod->removedOperators.insert(targetNick);
            mod->ChangedFlags += "o";
            mod->ChangedParams += " " + targetNick;
          }
        }
        break;
      case 'l': // ユーザー数制限
        if (isAdd) {
          if (it + 1 != modeArgs.end()) {
            const std::string &limitStr = *(++it);
            int limitValue;
            int ret = is_valid_limits_arg(limitStr, &limitValue);
            if (ret != 0) {
              return ret;
            }
            mod->modeFlags |= IChannelAggregateRoot::MODE_LIMIT_USERS;
            mod->newChannelLimit = limitValue;
            mod->ChangedFlags += "l";
            mod->ChangedParams += " " + limitStr;
          }
        } else {
          mod->modeFlags &= ~IChannelAggregateRoot::MODE_LIMIT_USERS;
          mod->newChannelLimit = 0;
          mod->ChangedFlags += "l";
        }
        break;
      case ' ':
        continue;
        break;
      default:
        return MessageConstants::ResponseCode::ERR_UNKNOWNMODE;
        break;
      }
    }
    isAdd = true;
  }
  return 0;
}

SendMsgDTO Mode::_handleUserMode(IMessageAggregateRoot *msg, IClientAggregateRoot *client) {
  // MessageStreamVector streams;
  // MessageStream stream = MessageService::generateMessageStream(this->_socketHandler, client);
  // stream << Message(
  //     ConfigsServiceLocator::get().getConfigs().Global.Name,
  //     MessageConstants::ResponseCode::ERR_NOSUCHNICK,
  //     client->getNickName() + " " + channelName + " :No such nick or channel name");
  // streams.push_back(stream);
  // return SendMsgDTO(0, streams);
  MessageStreamVector streams;
  (void)msg;
  (void)client;
  // ユーザーモード処理の実装
  return SendMsgDTO(0, streams);
}
