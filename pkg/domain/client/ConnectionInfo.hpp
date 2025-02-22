#ifndef CONNECTIONINFO_HPP
#define CONNECTIONINFO_HPP

#include "domain/shared/values/IHashAggregateRoot.hpp"
#include <poll.h>
#include <string>

class ConnectionInfo {
public:
  ConnectionInfo();
  ConnectionInfo(std::string addr, pollfd pollfd);
  ~ConnectionInfo();
  ConnectionInfo(const ConnectionInfo &other);
  ConnectionInfo &operator=(const ConnectionInfo &other);

  bool operator==(const ConnectionInfo &other) const;

  const int &getSocketFd() const;
  const std::string &getAddress() const;
  const pollfd &getPollfd() const;

private:
  int _sock;
  std::string _addr;
  pollfd _pollfd;
};

#endif /* CONNECTIONINFO_HPP */
