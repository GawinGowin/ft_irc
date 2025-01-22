#ifndef CLIENT_HPP
#define CLIENT_HPP

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

  const int &getId() const;
  const int &getSocketFd() const;
  const pollfd &getPollfd() const;
  bool operator==(const IClientAggregateRoot &other) const;

private:
  int _id;
  int _socketFd;
  pollfd _pollfd;
};

#endif /* CLIENT_HPP */
