#ifndef SOCKETHANDLER_HPP
#define SOCKETHANDLER_HPP

#include "domain/shared/ISocketHandler.hpp"
#include <arpa/inet.h>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

class SocketHandler : public virtual ISocketHandler {
public:
  SocketHandler(
      std::string address, const int port, const int maxConnections, const int maxBufferSize);
  ~SocketHandler();
  SocketHandler(const SocketHandler &other);
  SocketHandler &operator=(const SocketHandler &other);

  void initializeSocket();

  int acceptConnection();
  void closeConnection(int &targetSocket);
  void sendMsg(const std::string &message, int &targetSocket);
  std::string receiveMsg(int &targetSocket);

  void setPort(const int port);
  void setMaxConnections(const int maxConnections);
  void setMaxBufferSize(const int maxBufferSize);
  const int &getServerSocket() const;
  const int &getPort() const;
  const int &getMaxConnections() const;
  const int &getMaxBufferSize() const;
  const bool &isListening() const;

private:
  int _socket;
  int _port;
  int _maxBufferSize;
  int _maxConnections;
  int _currentConnections;

  struct sockaddr_in _addr;
  bool _isListening;

  SocketHandler();
};

#endif /* SOCKETHANDLER_HPP */
