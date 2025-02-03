#ifndef SOCKETHANDLER_HPP
#define SOCKETHANDLER_HPP

#include "domain/shared/ISocketHandler.hpp"
#include "domain/shared/values/IHashAggregateRoot.hpp"
#include <arpa/inet.h>
#include <poll.h>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

class SocketHandler : virtual public ISocketHandler {
public:
  SocketHandler(
      std::string address,
      const int port,
      const int maxConnections,
      const int maxBufferSize,
      IHashAggregateRoot *password);
  ~SocketHandler();
  SocketHandler(const SocketHandler &other);
  SocketHandler &operator=(const SocketHandler &other);

  void initializeSocket();

  void createPoll(pollfd *fds, nfds_t nfds);

  int acceptConnection();
  void closeConnection(int &targetSocket);
  void sendMsg(const std::string &message, int &targetSocket);
  std::string receiveMsg(const int &targetSocket);

  void setMaxConnections(const int maxConnections);
  void setMaxBufferSize(const int maxBufferSize);
  const int &getServerSocket() const;
  const int &getPort() const;
  const int &getMaxConnections() const;
  const int &getMaxBufferSize() const;
  const bool &isListening() const;
  const pollfd &getServerPollfd() const;

private:
  int _socket;
  int _port;
  int _maxBufferSize;
  int _maxConnections;
  int _currentConnections;
  bool _isListening;

  pollfd _serverPollfd;
  struct sockaddr_in _addr;

  IHashAggregateRoot *_password;

  SocketHandler();
};

#endif /* SOCKETHANDLER_HPP */
