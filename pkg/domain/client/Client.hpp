#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "IClientRepository.hpp"

class Client : virtual public IClientRepository {
  public:
    Client();
    ~Client();
    const int &getId() const;
  private:
    int _id;
};

#endif /* CLIENT_HPP */
