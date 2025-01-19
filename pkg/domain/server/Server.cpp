#include "Server.hpp"

Server::Server()
    : _addr(std::string()), _port(0), _clientFds(std::vector<int>()) {
  this->_srvPass = NULL;
  this->_srvConn = NULL;
}

Server::~Server() {
  if (this->_srvPass != NULL) {
    delete this->_srvPass;
  }
  if (this->_srvConn != NULL) {
    delete this->_srvConn;
  }
}

void Server::establishConnection() {
  try {
    this->_srvConn = new ServerConnection(this->_addr, this->_port);
  } catch (std::runtime_error &e) {
    throw std::runtime_error(std::string("Server: ") + e.what());
  }
}

void Server::setParams(const std::string &addr, const int &port) {
  this->_addr = addr;
  this->_port = port;
}

void Server::setPassword(const std::string &password) {
  if (this->_srvPass != NULL) {
    delete this->_srvPass;
  }
  this->_srvPass = new ServerPassword(password);
}

bool Server::isValidPassword(const std::string &password) {
  if (this->_srvPass == NULL) {
    return false;
  }
  return ServerPassword(password) == *(this->_srvPass);
}

void Server::registerClientById(const int &clientId) {
  std::vector<int>::iterator it;
  it = std::find(this->_clientFds.begin(), this->_clientFds.end(), clientId);
  if (it == this->_clientFds.end()) {
    this->_clientFds.push_back(clientId);
  } else {
    throw std::runtime_error("Server: client already exists");
  }
}

void Server::deleteClientById(const int &clientId) {
  std::vector<int>::iterator it;
  it = std::find(this->_clientFds.begin(), this->_clientFds.end(), clientId);
  if (it != this->_clientFds.end()) {
    this->_clientFds.erase(it);
  } else {
    throw std::runtime_error("Server: client does not exist");
  }
}
