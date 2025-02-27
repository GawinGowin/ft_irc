#include "application/commands/Join.hpp"

typedef std::vector<std::string>::iterator iter;

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
    // TODO: leave all channels
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
    if (channels[i] == "") {
      return exit_with_error("Syntax error", *client);
    }
    IChannelAggregateRoot *channel = db.get(channels[i]);
    if (channel == NULL) {
      db.add(Channel(channels[i]));
      channel = db.get(channels[i]);
    }
    if (channel->getListConnects().addClient(client->getNickName()) == 1) {
      logger->debugss() << "[JOIN] by (fd: " << client->getSocketFd() << "): already joined to "
                        << channels[i];
    } else {
      logger->debugss() << "[JOIN] by (fd: " << client->getSocketFd() << "): success to "
                        << channels[i];
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
