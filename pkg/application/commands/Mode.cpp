#include "application/commands/Mode.hpp"

Mode::Mode(IMessageAggregateRoot *msg, IClientAggregateRoot *client) : ACommands(msg, client) {}

SendMsgDTO Mode::execute() {
  MultiLogger *logger = LoggerServiceLocator::get();

  IMessageAggregateRoot *msg = this->getMessage();
  IClientAggregateRoot *client = this->getClient();

  ConfigsLoader &conf = ConfigsServiceLocator::get();
  InmemoryChannelDatabase &channelDB = InmemoryChannelDBServiceLocator::get();
  ISocketHandler &socketHandler = SocketHandlerServiceLocator::get();

  if (msg->getParams().size() < 1) {
    MessageStreamVector streams;
    MessageStream stream = MessageService::generateMessageStream(&socketHandler, client);
    stream << Message(
        ConfigsServiceLocator::get().getConfigs().Global.Name,
        MessageConstants::ResponseCode::ERR_NEEDMOREPARAMS,
        client->getNickName() + " MODE" + " :Syntax error");
    streams.push_back(stream);
    logger->debugss() << "[MODE]: err syndax (" << client->getSocketFd() << ")";
    return SendMsgDTO(1, streams);
  }
  std::string target = msg->getParams()[0];
  if (conf.getConfigs().Options.AllowedChannelTypes.find(target[0]) !=
      std::string::npos) { // 引数が"#"もしくは"&"で始まる場合
    return this->_handleChannelMode(socketHandler, channelDB);
  }
  return this->_handleUserMode(socketHandler, channelDB);
}

SendMsgDTO
Mode::_handleChannelMode(ISocketHandler &socketHandler, InmemoryChannelDatabase &channelDB) {
  IMessageAggregateRoot *msg = this->getMessage();
  IClientAggregateRoot *client = this->getClient();
  MultiLogger *logger = LoggerServiceLocator::get();

  MessageStreamVector streams;

  const std::string &channelName = msg->getParams()[0];
  IChannelAggregateRoot *channel = channelDB.get(channelName);
  if (!channel) {
    MessageStream stream = MessageService::generateMessageStream(&socketHandler, client);
    stream << Message(
        ConfigsServiceLocator::get().getConfigs().Global.Name,
        MessageConstants::ResponseCode::ERR_NOSUCHNICK,
        client->getNickName() + " " + channelName + " :No such nick or channel name");
    streams.push_back(stream);
    logger->debugss() << "[MODE]: no such channel (" << client->getSocketFd() << ")";
    return SendMsgDTO(1, streams);
  }
  if (msg->getParams().size() == 1) { // チャンネルのモードを表示
    std::string modeString = "+";
    int modeFlags = channel->getModeFlags();
    if (modeFlags & IChannelAggregateRoot::MODE_INVITE_ONLY) { // TODO: 他にモードがあるなら追加
      modeString += "i";
    }
    if (modeFlags & IChannelAggregateRoot::MODE_KEY_PROTECTED) {
      modeString += "k";
    }
    if (modeFlags & IChannelAggregateRoot::MODE_LIMIT_USERS) {
      modeString += "l";
    }
    std::stringstream ss;
    ss << channel->getCreationTime();
    std::string creationTimeStr = ss.str();

    Message modeResponse = Message(
        ConfigsServiceLocator::get().getConfigs().Global.Name,
        MessageConstants::ResponseCode::RPL_CHANNELMODEIS,
        client->getNickName() + " " + channelName + " " + modeString);
    Message creationtimeResponse = Message(
        ConfigsServiceLocator::get().getConfigs().Global.Name,
        MessageConstants::ResponseCode::RPL_CREATIONTIME_MSG,
        client->getNickName() + " " + channelName + " " + creationTimeStr);
    streams.push_back(MessageStream(&socketHandler, client) << modeResponse);
    streams.push_back(MessageStream(&socketHandler, client) << creationtimeResponse);
    logger->debugss() << "[MODE]: show mode (" << client->getSocketFd() << ")";
    return SendMsgDTO(0, streams);
  }
  if (!this->_is_channelOperator(channelName, client->getNickName())) {
    MessageStream stream = MessageService::generateMessageStream(&socketHandler, client);
    stream << Message(
        ConfigsServiceLocator::get().getConfigs().Global.Name,
        MessageConstants::ResponseCode::ERR_CHANOPRIVSNEEDED,
        client->getNickName() + " " + channelName + " :You are not channel operator");
    streams.push_back(stream);
    logger->debugss() << "[MODE]: not channel operator (" << client->getSocketFd() << ")";
    return SendMsgDTO(1, streams);
  }
  // チャンネルのモードを変更
  const std::string &modeString = msg->getParams()[1];
  int modeFlags;
  int newModeFlags = modeFlags = channel->getModeFlags();

  std::string response = channelName + " ";
  std::string modeChanges = "";

  /* TODO: ここからのパース部分の確認を行う */
  size_t paramIndex = 2; // MODE <channel> <modes> [args...]
  for (size_t i = 0; i < modeString.length(); i++) {
    bool isAdd = true;
    char c = modeString[i];

    if (c == '+') {
      isAdd = true;
      modeChanges += "+";
      continue;
    } else if (c == '-') {
      isAdd = false;
      modeChanges += "-";
      continue;
    }

    // 各モードの処理
    switch (c) {
    case 'i': // invite-only
      if (isAdd) {
        newModeFlags |= IChannelAggregateRoot::MODE_INVITE_ONLY;
        modeChanges += "i";
      } else {
        newModeFlags &= ~IChannelAggregateRoot::MODE_INVITE_ONLY;
        modeChanges += "i";
      }
      break;

    case 't': // topic制限
      // ここではIChannelAggregateRootでtモードの定数が定義されていないため、追加が必要です
      // 例: MODE_TOPIC_RESTRICTED = 1 << 3 のような定数を追加する必要があります
      break;

    case 'k': // キー（パスワード）
      if (isAdd) {
        if (paramIndex < msg->getParams().size()) {
          const std::string &key = msg->getParams()[paramIndex++];
          // チャンネルキーの設定（チャンネルクラスのAPIに応じて実装）
          // channel->setKey(key);
          newModeFlags |= IChannelAggregateRoot::MODE_KEY_PROTECTED;
          modeChanges += "k";
          response += " " + key;
        }
      } else {
        // キー削除
        // channel->setKey("");
        newModeFlags &= ~IChannelAggregateRoot::MODE_KEY_PROTECTED;
        modeChanges += "k";
      }
      break;

    case 'o': // オペレータ権限の付与/剥奪
      if (paramIndex < msg->getParams().size()) {
        const std::string &targetNick = msg->getParams()[paramIndex++];
        // オペレータ権限の設定（チャンネルクラスのAPIに応じて実装）
        // ステップ4で実装
        modeChanges += "o";
        response += " " + targetNick;
      }
      break;

    case 'l': // ユーザー数制限
      if (isAdd) {
        if (paramIndex < msg->getParams().size()) {
          const std::string &limitStr = msg->getParams()[paramIndex++];
          try {
            unsigned long limit = std::atol(limitStr.c_str());
            channel->setMaxUsers(limit);
            newModeFlags |= IChannelAggregateRoot::MODE_LIMIT_USERS;
            modeChanges += "l";
            response += " " + limitStr;
          } catch (...) {
            // 数値変換エラー
          }
        }
      } else {
        channel->setMaxUsers(0); // 制限なし
        newModeFlags &= ~IChannelAggregateRoot::MODE_LIMIT_USERS;
        modeChanges += "l";
      }
      break;
    }
  }

  // モードフラグを更新
  if (modeFlags != newModeFlags) {
    channel->setModeFlags(newModeFlags);
  }
  if (!modeChanges.empty()) {
    std::stringstream ss;
    ss << Message(
        client->getNickName() + "!" + client->getUserName() + "@" + client->getAddress(),
        MessageConstants::MODE, response);
    InmemoryClientDatabase &clientDB = InmemoryClientDBServiceLocator::get();
    MessageStreamVector newStreams = MessageService::generateMessageToChannel(
        &socketHandler, client, &clientDB, channel, ss.str());
    streams.insert(streams.end(), newStreams.begin(), newStreams.end());
    MessageStream stream = MessageService::generateMessageStream(&socketHandler, client);
    streams.push_back(stream << ss.str());
    logger->debugss() << "[MODE]: change mode (" << client->getSocketFd() << ")";
  }
  return SendMsgDTO(0, streams);
}

SendMsgDTO
Mode::_handleUserMode(ISocketHandler &socketHandler, InmemoryChannelDatabase &channelDB) {
  MessageStreamVector streams;
  (void)socketHandler;
  (void)channelDB;
  // ユーザーモード処理の実装

  return SendMsgDTO(0, streams);
}

bool Mode::_is_channelOperator(const std::string &channel, const std::string &nickname) {
  (void)channel;
  (void)nickname;
  return true;
}
