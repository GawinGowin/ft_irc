#ifndef ICLIENTAGGREGATEROOT_HPP
#define ICLIENTAGGREGATEROOT_HPP

#include <poll.h>
#include <string>

#define CLIENT_UNKNOWN 0x0001 // 未登録
#define CLIENT_GOTPASS 0x0002 // PASS入力済
#define CLIENT_GOTNICK 0x0004 // NICK入力済
#define CLIENT_GOTUSER 0x0008 // USER入力済
#define CLIENT_USER 0x000F    // USER登録済
#define CLIENT_NONPASS 0x000D // PASS未入力でNICK/USER入力済

class IClientAggregateRoot {
public:
  virtual ~IClientAggregateRoot(){};

  virtual bool operator==(const IClientAggregateRoot &other) const = 0;

  virtual IClientAggregateRoot *clone() const = 0;

  virtual const std::string &getId() const = 0;
  virtual const std::string &getNickName() const = 0;
  virtual const std::string &getUserName() const = 0;
  virtual const std::string &getPassword() const = 0;
  virtual const int &getSocketFd() const = 0;
  virtual const pollfd &getPollfd() const = 0;
  virtual const std::string &getAddress() const = 0;

  virtual void setId(const std::string &id) = 0;
  virtual void setUserName(const std::string &userName) = 0;
  virtual void setNickName(const std::string &nickName) = 0;
  virtual int setPassword(const std::string &password) = 0;

  virtual int getClientType() = 0;
  virtual void setClientType(int clientType) = 0;
};

#endif /* ICLIENTAGGREGATEROOT_HPP */
