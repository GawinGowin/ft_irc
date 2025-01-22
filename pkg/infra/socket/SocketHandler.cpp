#include "infra/socket/SocketHandler.hpp"

SocketHandler::SocketHandler(
    const std::string address,
    const int port,
    const int maxConnections,
    const int maxBufferSize,
    IServerPasswordAggregateRoot *password)
    : _socket(-1), _port(port), _maxBufferSize(maxBufferSize), _maxConnections(maxConnections),
      _currentConnections(0), _isListening(false), _password(password) {
  this->_addr.sin_family = AF_INET;
  this->_addr.sin_port = htons(port);
  this->_addr.sin_addr.s_addr = inet_addr(address.c_str());
  this->_serverPollfd = {0, 0, 0};
}

SocketHandler::~SocketHandler() {
  if (this->_socket != -1) {
    close(this->_socket);
    this->_socket = -1;
  }
  if (this->_password != NULL) {
    delete this->_password;
    this->_password = NULL;
  }
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
  this->_serverPollfd = {this->_socket, POLLIN, 0};
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
  char send_buf = 0;
  if (send(targetSocket, &send_buf, 1, 0) == -1) {
    throw std::runtime_error("send failed");
  }
  close(targetSocket);
  this->_currentConnections--;
}

int SocketHandler::acceptConnection() {
  if (!this->_isListening) {
    throw std::runtime_error("socket is not listening");
  }
  if (this->_currentConnections >= this->_maxConnections) {
    throw std::runtime_error("max connections reached");
  }
  int clientSocket = accept(this->_socket, NULL, NULL);
  if (clientSocket == -1) {
    throw std::runtime_error("accept failed");
  }
  this->_currentConnections++;
  return clientSocket;
}

void SocketHandler::sendMsg(const std::string &message, int &targetSocket) {
  if (!this->_isListening) {
    throw std::runtime_error("socket is not listening");
  }
  if (send(targetSocket, message.c_str(), message.size(), 0) == -1) {
    throw std::runtime_error("send failed");
  }
}

std::string SocketHandler::receiveMsg(int &targetSocket) {
  if (!this->_isListening) {
    throw std::runtime_error("socket is not listening");
  }
  char recv_buf[this->_maxBufferSize];
  ssize_t recv_size = recv(targetSocket, recv_buf, this->_maxBufferSize, 0);
  if (recv_size == -1) {
    throw std::runtime_error("recieve msg failed");
  }
  return std::string(recv_buf, recv_size);
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
