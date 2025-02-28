#ifndef ICLIENTAGGREGATEROOT_HPP
#define ICLIENTAGGREGATEROOT_HPP

#include <poll.h>
#include <string>

class IClientAggregateRoot {
public:
  virtual ~IClientAggregateRoot(){};

  virtual bool operator==(const IClientAggregateRoot &other) const = 0;

  virtual IClientAggregateRoot *clone() const = 0;

  virtual const std::string &getId() const = 0;
  virtual const std::string &getNickName() const = 0;
  virtual const std::string &getPassword() const = 0;
  virtual const int &getSocketFd() const = 0;
  virtual const pollfd &getPollfd() const = 0;
  virtual const std::string &getAddress() const = 0;

  virtual void setId(const std::string &id) = 0;
  virtual void setNickName(const std::string &nickName) = 0;
  virtual int setPassword(const std::string &password) = 0;
};

#endif /* ICLIENTAGGREGATEROOT_HPP */
