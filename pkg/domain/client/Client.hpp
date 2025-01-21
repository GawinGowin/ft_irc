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
  bool operator==(const Client &other) const { return this->_id == other._id; }

private:
  int _id;
};

#endif /* CLIENT_HPP */
