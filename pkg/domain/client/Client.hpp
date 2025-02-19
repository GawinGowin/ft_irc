#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "domain/client/ConnectionInfo.hpp"
#include "domain/client/IClientAggregateRoot.hpp"
#include <poll.h>
#include <stdexcept>

class Client : virtual public IClientAggregateRoot {
public:
  Client();
  Client(int id, pollfd pollfd);
  ~Client();
  Client(const Client &other);
  Client &operator=(const Client &other);

  Client *clone() const;

  const int &getId() const;
  const int &getSocketFd() const;
  const pollfd &getPollfd() const;
  bool operator==(const IClientAggregateRoot &other) const;
  const std::string &getNickName() const;
  void setNickName(const std::string &nickName);

  void setConnectionInfo(const ConnectionInfo &connectionInfo);

private:
  int _id;
  int _socketFd;
  pollfd _pollfd;

  ConnectionInfo _connectionInfo;
  std::string _nickName;
};

#endif /* CLIENT_HPP */
