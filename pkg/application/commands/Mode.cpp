#include "application/commands/Mode.hpp"

inline static SendMsgDTO handleChannelMode();
inline static SendMsgDTO handleUserMode();
inline static bool ChannelOperator(const std::string &channel, const std::string &nickname);

Mode::Mode(IMessageAggregateRoot *msg, IClientAggregateRoot *client) : ACommands(msg, client) {}

SendMsgDTO Mode::execute() {
  MultiLogger *logger = LoggerServiceLocator::get();
  IMessageAggregateRoot *msg = this->getMessage();
  IClientAggregateRoot *client = this->getClient();

  MessageStreamVector streams;
  ISocketHandler *socketHandler = &SocketHandlerServiceLocator::get();

  if (msg->getParams().size() < 1) {
    return SendMsgDTO(1, streams); // TODO: エラーメッセージを後で設定
  }
  std::string target = msg->getParams()[0];
  if (target[0] == '#' || target[0] == '&') { // TODO: Configから可能なチャンネル名を読み取るかも
    return handleChannelMode();
  }
  return handleUserMode();
}

inline static SendMsgDTO handleChannelMode() {
  // チャンネルモード処理の実装


  return SendMsgDTO(0, MessageStreamVector());
}

inline static SendMsgDTO handleUserMode() {
  // ユーザーモード処理の実装


  return SendMsgDTO(0, MessageStreamVector());
}

inline static bool ChannelOperator(const std::string &channel, const std::string &nickname) {
  return true;
}
