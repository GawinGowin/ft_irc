#ifndef SERVER_HPP
#define SERVER_HPP

#include "domain/server/IServerAggregateRoot.hpp"
#include "domain/server/ServerPassword.hpp"
#include "domain/shared/ISocketHandler.hpp"
#include <algorithm>
#include <vector>

class Server : virtual public IServerAggregateRoot {
public:
  // Server(const std::string &addr, const int &port, const int &buf_size);
  Server(ISocketHandler *socketHandler, const std::string &password);
  ~Server();

  bool isValidPassword(const std::string &password);

  std::string getPasswordAsHash();

  const int &getServerSocket() const;
  const bool &isListening() const;

private:
  ISocketHandler *_socketHandler;
  const ServerPassword _srvPass;

  Server(const Server &obj);
  Server &operator=(const Server &obj);
};

#endif /* SERVER_HPP */
