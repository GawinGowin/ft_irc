// #include "application/commands/Join.hpp"
// #include "application/serviceLocator/InmemoryClientDBServiceLocator.hpp"
// #include "application/serviceLocator/SocketHandlerServiceLocator.hpp"
// #include "domain/message/MessageService.hpp"

// typedef std::vector<std::string>::iterator iter;

// static inline bool checkChannelName(const std::string &channelName);
// static inline void split(const std::string &s, char delim, std::vector<std::string> &elems);
// static inline SendMsgDTO exit_with_error(const std::string &msg, IClientAggregateRoot &client);
// static inline void broadcastJoinToChannel(ISocketHandler *socketHandler, IClientRepository *clientRepo,
//                                          IClientAggregateRoot *client, IChannelAggregateRoot *channel);
// static inline std::string generateChannelInfoResponse(IClientAggregateRoot *client,
//                                                      IChannelAggregateRoot *channel);

// Join::Join(IMessageAggregateRoot *msg, IClientAggregateRoot *client) : ACommands(msg, client) {}

// SendMsgDTO Join::execute() {
//   MultiLogger *logger = LoggerServiceLocator::get();
//   IMessageAggregateRoot *msg = this->getMessage();
//   IClientAggregateRoot *client = this->getClient();
//   InmemoryChannelDatabase &channelDB = InmemoryChannelDBServiceLocator::get();
//   InmemoryClientDatabase *clientDB = &InmemoryClientDBServiceLocator::get();
//   ISocketHandler *socketHandler = &SocketHandlerServiceLocator::get();

//   // 基本的なチェック
//   if (client->getNickName() == "") {
//     return exit_with_error("You must set your nickname first", *client);
//   }
//   if (msg->getParams().size() < 1 || msg->getParams().size() > 2) {
//     return exit_with_error("Invalid number of parameters", *client);
//   }

//   // 0を指定した場合は、すべてのチャンネルから退出
//   if (msg->getParams()[0] == "0") {
//     std::vector<std::string> joinedChannels;
//     const IdToChannelMap &db = channelDB.getDatabase();
//     IdToChannelMap::const_iterator it;
//     for (it = db.begin(); it != db.end(); ++it) {
//       IChannelAggregateRoot *channel = (*it).second;
//       if (channel->getListConnects().hasClient(client->getNickName())) {
//         channel->getListConnects().removeClient(client->getNickName());
//         joinedChannels.push_back((*it).first);
//         logger->debugss() << "[PART] by (fd: " << client->getSocketFd() << "): left "
//                           << (*it).first;
//       }
//     }
//     return SendMsgDTO(); // 成功の場合は空のレスポンス
//   }

//   // チャンネル名とパスワードの解析
//   std::vector<std::string> channels;
//   std::vector<std::string> channelPasswords;
//   split(msg->getParams()[0], ',', channels);
//   if (msg->getParams().size() == 2) {
//     split(msg->getParams()[1], ',', channelPasswords);
//     if (channels.size() != channelPasswords.size()) {
//       return exit_with_error("Syntax error", *client);
//     }
//   }

//   // 全チャンネルの応答を結合するためのストリーム
//   std::stringstream combinedResponse;

//   for (size_t i = 0; i < channels.size(); i++) {
//     // チャンネル名の検証
//     if (!checkChannelName(channels[i])) {
//       logger->debugss() << "[JOIN] by (fd: " << client->getSocketFd() << "): Invalid channel name: "
//                         << channels[i];
//       continue; // 無効なチャンネル名はスキップ
//     }

//     // パスワードの取得
//     std::string password = "";
//     if (i < channelPasswords.size()) {
//       password = channelPasswords[i];
//     }

//     // チャンネルの取得または作成
//     IChannelAggregateRoot *channel = channelDB.get(channels[i]);
//     if (channel == NULL) {
//       // 新しいチャンネルを作成
//       channelDB.add(Channel(channels[i]));
//       channel = channelDB.get(channels[i]);
//     } else {
//       // 既存チャンネルのチェック
//       if (!channel->getKey().empty() && channel->getKey() != password) {
//         logger->debugss() << "[JOIN] by (fd: " << client->getSocketFd() << "): Invalid key for "
//                          << channels[i];
//         continue; // パスワードが不正な場合はスキップ
//       }

//       // ユーザー数制限のチェック
//       if (channel->getMaxUsers() > 0 &&
//           channel->getListConnects().getClients().size() >= channel->getMaxUsers()) {
//         logger->debugss() << "[JOIN] by (fd: " << client->getSocketFd() << "): Channel is full: "
//                          << channels[i];
//         continue; // チャンネルが満員の場合はスキップ
//       }
//     }

//     // チャンネルへの参加
//     int joinResult = channel->getListConnects().addClient(client->getNickName());
//     if (joinResult == 1) {
//       logger->debugss() << "[JOIN] by (fd: " << client->getSocketFd() << "): already joined to "
//                         << channels[i];
//     } else {
//       logger->debugss() << "[JOIN] by (fd: " << client->getSocketFd() << "): success to "
//                         << channels[i];

//       // チャンネルメンバーにJOIN通知をブロードキャスト
//       broadcastJoinToChannel(socketHandler, clientDB, client, channel);

//       // チャンネル情報応答を生成
//       combinedResponse << generateChannelInfoResponse(client, channel);
//     }
//   }

//   // 応答の生成
//   SendMsgDTO dto;
//   if (!combinedResponse.str().empty()) {
//     dto.setMessage(combinedResponse.str());
//   }
//   return dto;
// }

// static inline void broadcastJoinToChannel(ISocketHandler *socketHandler, IClientRepository *clientRepo,
//                                          IClientAggregateRoot *client, IChannelAggregateRoot *channel) {
//   // JOIN通知メッセージを作成
//   std::stringstream joinMsg;
//   joinMsg << ":" << client->getNickName() << " JOIN " << channel->getName();

//   // まず自分自身にJOIN通知を送信
//   MessageStream selfStream = MessageService::generateMessageStream(socketHandler, client);
//   selfStream << joinMsg.str() << "\r\n";
//   selfStream.send();

//   // 他のメンバーにJOIN通知をブロードキャスト
//   std::vector<MessageStream> streams = MessageService::generateMessageToChannel(
//       socketHandler, client, clientRepo, channel, joinMsg.str());

//   // 各ストリームを送信
//   for (std::vector<MessageStream>::iterator it = streams.begin(); it != streams.end(); ++it) {
//     it->send();
//   }
// }

// static inline std::string generateChannelInfoResponse(IClientAggregateRoot *client,
//                                                      IChannelAggregateRoot *channel) {
//   std::stringstream response;

//   // トピックが設定されている場合は送信
//   const std::string &topic = channel->getTopic().getTopic();
//   if (!topic.empty()) {
//     response << ":server 332 " << client->getNickName() << " " << channel->getName() << " :"
//              << topic << "\r\n";
//   }

//   // チャンネルメンバーリストを送信 (NAMES)
//   const std::vector<std::string> &members = channel->getListConnects().getClients();
//   response << ":server 353 " << client->getNickName() << " = " << channel->getName() << " :";

//   for (std::vector<std::string>::const_iterator member = members.begin();
//        member != members.end(); ++member) {
//     if (member != members.begin()) {
//       response << " ";
//     }
//     response << *member;
//   }
//   response << "\r\n";

//   // NAMES終了の通知
//   response << ":server 366 " << client->getNickName() << " " << channel->getName()
//            << " :End of /NAMES list.\r\n";

//   return response.str();
// }

// static inline void split(const std::string &s, char delim, std::vector<std::string> &elems) {
//   std::stringstream ss(s);
//   std::string item;
//   while (std::getline(ss, item, delim)) {
//     elems.push_back(item);
//   }
// }

// static inline SendMsgDTO exit_with_error(const std::string &msg, IClientAggregateRoot &client) {
//   MultiLogger *logger = LoggerServiceLocator::get();
//   SendMsgDTO dto;
//   dto.setStatus(1);

//   // エラーメッセージをIRCプロトコル形式で設定
//   std::stringstream errorMsg;
//   errorMsg << ":server 403 " << client.getNickName() << " :" << msg << "\r\n";
//   dto.setMessage(errorMsg.str());

//   logger->debugss() << "[JOIN] by (fd: " << client.getSocketFd() << "): " << msg;
//   return dto;
// }

// static inline bool checkChannelName(const std::string &channelName) {
//   if (channelName.empty() || channelName.length() > 50) {
//     return false;
//   }

//   // チャンネル名は'#'または'&'で始まる必要がある
//   if (channelName[0] != '#' && channelName[0] != '&') {
//     return false;
//   }

//   // 禁止文字のチェック（スペース、カンマ、コロン、CR、LF等）
//   const std::string invalidChars = " ,:\r\n";
//   if (channelName.find_first_of(invalidChars) != std::string::npos) {
//     return false;
//   }

//   return true;
// }