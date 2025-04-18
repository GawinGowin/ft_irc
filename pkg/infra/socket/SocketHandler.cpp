#include "infra/socket/SocketHandler.hpp"

static bool isValidIPAddress(const std::string &ipStr);

SocketHandler::SocketHandler(
    const std::string address, const int port, const int maxConnections, const int maxBufferSize)
    : _socket(-1), _port(port), _maxBufferSize(maxBufferSize), _maxConnections(maxConnections),
      _currentConnections(0), _isListening(false) {
  if (!isValidIPAddress(address)) {
    throw std::runtime_error("invalid address");
  } else if (port < 0 || port > 65535) {
    throw std::runtime_error("invalid port");
  } else if (maxConnections < 0 || maxConnections > SOMAXCONN) {
    throw std::runtime_error("invalid max connections");
  } else if (maxBufferSize < 0 || maxBufferSize > 65535) {
    throw std::runtime_error("invalid max buffer size (<= 65535)");
  }
  this->_addr.sin_family = AF_INET;
  this->_addr.sin_port = htons(port);
  this->_addr.sin_addr.s_addr = inet_addr(address.c_str());
  this->_serverPollfd.fd = 0;
  this->_serverPollfd.events = 0;
  this->_serverPollfd.revents = 0;
  this->_receiveBuffers.clear();
}

SocketHandler::~SocketHandler() {
  if (this->_socket != -1) {
    close(this->_socket);
    this->_socket = -1;
  }
  this->_receiveBuffers.clear();
}

SocketHandler::SocketHandler(const SocketHandler &other) { *this = other; }

SocketHandler &SocketHandler::operator=(const SocketHandler &other) {
  if (this != &other) {
    this->_socket = other._socket;
    this->_port = other._port;
    this->_maxConnections = other._maxConnections;
    this->_maxBufferSize = other._maxBufferSize;
    this->_isListening = other._isListening;
    this->_currentConnections = other._currentConnections;
    this->_addr.sin_family = other._addr.sin_family;
    this->_addr.sin_port = other._addr.sin_port;
    this->_addr.sin_addr.s_addr = other._addr.sin_addr.s_addr;
    this->_serverPollfd.events = other._serverPollfd.events;
    this->_serverPollfd.revents = other._serverPollfd.revents;
    this->_serverPollfd.fd = other._serverPollfd.fd;
    this->_receiveBuffers = other._receiveBuffers;
  }
  return *this;
}

void SocketHandler::initializeSocket() {
  this->_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (this->_socket == -1) {
    throw std::runtime_error("socket failed");
  }
  if (bind(this->_socket, (const struct sockaddr *)&this->_addr, sizeof(this->_addr)) == -1) {
    throw std::runtime_error("bind failed");
  }
  if (listen(this->_socket, SOMAXCONN) == -1) {
    throw std::runtime_error("listen failed");
  }
  this->_serverPollfd.fd = this->_socket;
  this->_serverPollfd.events = POLLIN;
  this->_serverPollfd.revents = 0;
  this->_isListening = true;
}

void SocketHandler::createPoll(pollfd *fds, nfds_t nfds) {
  if (!this->_isListening) {
    throw std::runtime_error("socket is not listening");
  }
  if (poll(fds, nfds, -1) < 0) {
    throw std::runtime_error("poll failed");
  }
}

void SocketHandler::closeConnection(int &targetSocket) {
  if (!this->_isListening) {
    throw std::runtime_error("socket is not listening");
  }
  this->_receiveBuffers.erase(targetSocket);
  close(targetSocket);
  this->_currentConnections--;
}

int SocketHandler::acceptConnection(struct sockaddr_in *clientAddr) {
  if (!this->_isListening) {
    throw std::runtime_error("socket is not listening");
  }
  if (this->_currentConnections >= this->_maxConnections) {
    return -1;
  }
  int clientSocket = -1;
  if (clientAddr == NULL) {
    clientSocket = accept(this->_socket, NULL, NULL);
  } else {
    socklen_t clientAddrLen = sizeof(clientAddr);
    clientSocket =
        accept(this->_socket, reinterpret_cast<struct sockaddr *>(clientAddr), &clientAddrLen);
  }
  if (clientSocket == -1) {
    throw std::runtime_error("accept failed");
  }
  this->_currentConnections++;
  return clientSocket;
}

ssize_t SocketHandler::sendMsg(const std::string &message, int &targetSocket) {
  if (!this->_isListening) {
    return (-1);
  }
  return send(targetSocket, message.c_str(), message.size(), MSG_NOSIGNAL);
}

std::string SocketHandler::receiveMsg(const int &targetSocket) {
  if (!this->_isListening) {
    throw std::runtime_error("socket is not listening");
  }

  std::string delimiter = "\r\n";
  std::string &buffer = this->_receiveBuffers[targetSocket];

  size_t pos;
  char recv_buf[this->_maxBufferSize];
  ssize_t bytes_received;

  bytes_received = recv(targetSocket, recv_buf, this->_maxBufferSize - 1, 0);
  if (bytes_received > 0) {
    recv_buf[bytes_received] = '\0';
    buffer += std::string(recv_buf, bytes_received);

    pos = buffer.rfind(delimiter);
    if (pos != std::string::npos) {
      std::string message = buffer.substr(0, pos + delimiter.size());
      buffer.erase(0, pos + delimiter.size());
      return message;
    }
  } else if (bytes_received == 0) {
    buffer.clear();
    return "";
  }
  return buffer;
}

void SocketHandler::setMaxConnections(const int maxConnections) {
  this->_maxConnections = maxConnections;
}

void SocketHandler::setMaxBufferSize(const int maxBufferSize) {
  this->_maxBufferSize = maxBufferSize;
}

const int &SocketHandler::getServerSocket() const { return this->_socket; }

const int &SocketHandler::getPort() const { return this->_port; }

const int &SocketHandler::getMaxConnections() const { return this->_maxConnections; }

const int &SocketHandler::getMaxBufferSize() const { return this->_maxBufferSize; }

const bool &SocketHandler::isListening() const { return this->_isListening; }

const pollfd &SocketHandler::getServerPollfd() const { return this->_serverPollfd; }

std::string SocketHandler::getClientIp(const struct sockaddr_in &clientAddr) const {
  char clientIP[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, sizeof(clientIP));
  return std::string(clientIP);
}

static bool isValidIPAddress(const std::string &ipStr) {
  struct in_addr ipv4;
  struct in6_addr ipv6;

  if (inet_pton(AF_INET, ipStr.c_str(), &ipv4) == 1) {
    return true;
  }
  if (inet_pton(AF_INET6, ipStr.c_str(), &ipv6) == 1) {
    return false; // IPv6 is not supported
  }
  return false;
}
