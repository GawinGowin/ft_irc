#include "application/commands/Mode.hpp"

Mode::Mode(IMessageAggregateRoot *msg, IClientAggregateRoot *client) : ACommands(msg, client) {}

SendMsgDTO Mode::execute() {
  // MultiLogger *logger = LoggerServiceLocator::get();

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
    return SendMsgDTO(0, streams);
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
