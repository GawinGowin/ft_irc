#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "domain/client/ConnectionInfo.hpp"
#include "domain/client/IClientAggregateRoot.hpp"
#include "domain/client/Password.hpp"

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

  const std::string &getId() const;
  const std::string &getNickName() const;
  const std::string &getUserName() const;
  const std::string &getPassword() const;
  const int &getSocketFd() const;
  const pollfd &getPollfd() const;
  const std::string &getAddress() const;
  const std::string &getRealName() const;

  void setId(const std::string &id);
  void setUserName(const std::string &userName);
  void setNickName(const std::string &nickName);
  int setPassword(const std::string &password);
  void setRealName(const std::string &realName);

  int getClientType();
  void setClientType(int clientType);

private:
  int _id;
  std::string _nickName;
  std::string _userName;
  Password _password;
  ConnectionInfo _connectionInfo;
  int _clientType;
  std::string _realName;
};

#endif /* CLIENT_HPP */
