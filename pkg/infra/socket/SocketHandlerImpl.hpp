#ifndef SOCKETHANDLERIMPL_HPP
#define SOCKETHANDLERIMPL_HPP

#include "ISocketHandler.hpp"
#include <sys/socket.h>
#include <stdexcept>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>

class SocketHandlerImpl: public virtual ISocketHandler {
public:
  SocketHandlerImpl(std::string address, const int port, const int maxConnections, const int maxBufferSize);
  ~SocketHandlerImpl();

  void initializeSocket();

  int acceptConnection();
  void closeConnection(int &targetSocket);
  void sendMsg(const std::string &message, int &targetSocket);
  std::string receiveMsg(int &targetSocket);

  void setPort(const int port);
  void setMaxConnections(const int maxConnections);
  void setMaxBufferSize(const int maxBufferSize);
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

  SocketHandlerImpl();
  SocketHandlerImpl(const SocketHandlerImpl &other);
  SocketHandlerImpl &operator=(const SocketHandlerImpl &other);
};

#endif /* SOCKETHANDLERIMPL_HPP */
