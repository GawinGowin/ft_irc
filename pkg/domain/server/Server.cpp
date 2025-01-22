#include "domain/server/Server.hpp"

Server::Server(ISocketHandler *socketHandler, const std::string &password)
    : _socketHandler(socketHandler), _srvPass(ServerPassword(password)) {}

Server::~Server() {}

bool Server::isValidPassword(const std::string &password) {
  return ServerPassword(password) == this->_srvPass;
}

std::string Server::getPasswordAsHash() { return this->_srvPass.getHash(); }

const int &Server::getServerSocket() const { return this->_socketHandler->getServerSocket(); }

const bool &Server::isListening() const { return this->_socketHandler->isListening(); }