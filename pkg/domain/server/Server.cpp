#include "Server.hpp"

Server::Server(const std::string &addr, const int &port, const int &buf_size)
    : _addr(addr), _port(port), _buf_size(buf_size), _clientIds(std::vector<int>()) {
  this->_srvPass = NULL;
}

Server::~Server() {
  if (this->_srvPass != NULL) {
    delete this->_srvPass;
  }
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

void Server::addClientById(const int &clientId) {
  std::vector<int>::iterator it;
  it = std::find(this->_clientIds.begin(), this->_clientIds.end(), clientId);
  if (it == this->_clientIds.end()) {
    this->_clientIds.push_back(clientId);
  } else {
    throw std::runtime_error("Client already exists");
  }
}

void Server::removeClientById(const int &clientId) {
  std::vector<int>::iterator it;
  it = std::find(this->_clientIds.begin(), this->_clientIds.end(), clientId);
  if (it != this->_clientIds.end()) {
    this->_clientIds.erase(it);
  } else {
    throw std::runtime_error("Client does not exist");
  }
}
