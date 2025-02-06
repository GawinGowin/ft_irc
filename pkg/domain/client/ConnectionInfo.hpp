#ifndef CONNECTIONINFO_HPP
#define CONNECTIONINFO_HPP

#include "domain/shared/values/IHashAggregateRoot.hpp"
#include <string>

class ConnectionInfo {
public:
  ConnectionInfo();
  ConnectionInfo(int sock, std::string addr);
  ~ConnectionInfo();
  ConnectionInfo(const ConnectionInfo &other);
  ConnectionInfo &operator=(const ConnectionInfo &other);

  bool operator==(const ConnectionInfo &other) const;

  const int &getSocketFd() const;
  const std::string &getAddress() const;
  const std::string &getPassword() const;

private:
  int _sock;
  std::string _addr;
  std::string _pwd;
};

#endif /* CONNECTIONINFO_HPP */
