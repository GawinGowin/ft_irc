#include "SocketHandlerImpl.hpp"

SocketHandlerImpl::SocketHandlerImpl(
    const std::string address, const int port, const int maxConnections, const int maxBufferSize)
    : _socket(-1), _port(port), _maxConnections(maxConnections), _maxBufferSize(maxBufferSize),
      _isListening(false), _currentConnections(0) {
  this->_addr.sin_family = AF_INET;
  this->_addr.sin_port = htons(port);
  this->_addr.sin_addr.s_addr = inet_addr(address.c_str());
}

SocketHandlerImpl::~SocketHandlerImpl() {
  if (this->_socket != -1) {
    close(this->_socket);
    this->_socket = -1;
  }
}

void SocketHandlerImpl::initializeSocket() {
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
  this->_isListening = true;
}

void SocketHandlerImpl::closeConnection(int &targetSocket) {
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

int SocketHandlerImpl::acceptConnection() {
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

void SocketHandlerImpl::sendMsg(const std::string &message, int &targetSocket) {
  if (!this->_isListening) {
    throw std::runtime_error("socket is not listening");
  }
  if (send(targetSocket, message.c_str(), message.size(), 0) == -1) {
    throw std::runtime_error("send failed");
  }
}

std::string SocketHandlerImpl::receiveMsg(int &targetSocket) {
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

void SocketHandlerImpl::setMaxConnections(const int maxConnections) {
  this->_maxConnections = maxConnections;
}

void SocketHandlerImpl::setMaxBufferSize(const int maxBufferSize) {
  this->_maxBufferSize = maxBufferSize;
}

const int &SocketHandlerImpl::getPort() const { return this->_port; }

const int &SocketHandlerImpl::getMaxConnections() const { return this->_maxConnections; }

const int &SocketHandlerImpl::getMaxBufferSize() const { return this->_maxBufferSize; }

const bool &SocketHandlerImpl::isListening() const { return this->_isListening; }
