#ifndef SERVER_HPP
#define SERVER_HPP

#include "domain/server/IServerAggregateRoot.hpp"
#include "domain/server/ServerPassword.hpp"
#include "shared/ISocketHandler.hpp"
#include <algorithm>
#include <vector>

class Server : virtual public IServerAggregateRoot {
public:
  // Server(const std::string &addr, const int &port, const int &buf_size);
  Server(ISocketHandler *socketHandler, const std::string &password);
  ~Server();

  bool isValidPassword(const std::string &password);

  const std::string &getPasswordAsHash() const;

  const int &getServerSocket() const;
  const bool &isListening() const;

private:
  const ServerPassword _srvPass;

  ISocketHandler *_socketHandler;

  Server(const Server &obj);
  Server &operator=(const Server &obj);
};

#endif /* SERVER_HPP */
