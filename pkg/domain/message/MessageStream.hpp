#ifndef MESSAGESTREAM_HPP
#define MESSAGESTREAM_HPP

#include <sstream>
#include <string>

#include "domain/client/IClientAggregateRoot.hpp"
#include "infra/socket/ISocketHandler.hpp"

class MessageStream {
public:
  MessageStream(ISocketHandler *socketHandler, IClientAggregateRoot *client)
      : _socketHandler(socketHandler), _client(client) {}

  MessageStream(const MessageStream &other)
      : _socketHandler(other._socketHandler), _client(other._client), _stream(other._stream.str()) {
  }
  MessageStream &operator=(const MessageStream &other) {
    if (this != &other) {
      this->_socketHandler = other._socketHandler;
      this->_client = other._client;
      this->_stream.str(other._stream.str());
    }
    return *this;
  }

  ~MessageStream() {}

  template <typename T> MessageStream &operator<<(const T &value) {
    _stream << value;
    return *this;
  }
  MessageStream &operator<<(std::ostream &(*manip)(std::ostream &)) {
    _stream << manip;
    return *this;
  }

  int send() {
    if (this->_socketHandler->isListening() == false || this->_client == NULL) {
      return (-1);
    }
    int fd = this->_client->getSocketFd();
    if (fd == -1) {
      return (-1);
    }
    std::string msg = this->_stream.str();
    ssize_t sent = this->_socketHandler->sendMsg(msg, fd);
    if (sent != static_cast<ssize_t>(msg.size())) {
      return (-1);
    }
    return (0);
  }

  IClientAggregateRoot *getClient() { return this->_client; }

private:
  ISocketHandler *_socketHandler;
  IClientAggregateRoot *_client;
  std::ostringstream _stream;
};

#endif /* MESSAGESTREAM_HPP */
