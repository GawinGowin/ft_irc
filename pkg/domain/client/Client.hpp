#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "domain/client/IClientAggregateRoot.hpp"

class Client : virtual public IClientAggregateRoot {
public:
  Client();
  ~Client();
  Client(const Client &other);
  Client &operator=(const Client &other);

  const int &getId() const;
  const int &getSocketFd() const;
  bool operator==(const Client &other) const;

private:
  int _id;
  int _socketFd;
};

#endif /* CLIENT_HPP */
