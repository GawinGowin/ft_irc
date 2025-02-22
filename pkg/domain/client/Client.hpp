#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "domain/client/ConnectionInfo.hpp"
#include "domain/client/IClientAggregateRoot.hpp"
#include <poll.h>
#include <stdexcept>
#include <string>

class Client : virtual public IClientAggregateRoot {
public:
  Client();
  Client(std::string addr, pollfd pollfd);
  ~Client();
  Client(const Client &other);
  Client &operator=(const Client &other);

  bool operator==(const IClientAggregateRoot &other) const;

  Client *clone() const;

  const int &getId() const;
  const std::string &getNickName() const;
  const std::string &getPassword() const;
  const int &getSocketFd() const;
  const pollfd &getPollfd() const;

  void setId(const int &id);
  void setNickName(const std::string &nickName);
  void setPassword(const std::string &password);

private:
  int _id;
  std::string _nickName;
  std::string _password;

  ConnectionInfo _connectionInfo;
};

#endif /* CLIENT_HPP */
