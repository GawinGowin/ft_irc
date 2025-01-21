#ifndef ICLIENTREPOSITORY_HPP
#define ICLIENTREPOSITORY_HPP

#include "domain/client/Client.hpp"
#include <vector>

class IClientRepository {
  public:
    virtual ~IClientRepository() = 0;
    virtual void addClient(const Client &client) = 0;
    virtual const std::vector<Client> &getClients() = 0;
    virtual const Client &getClientById(const int id) = 0;
    virtual void popClientById(const int id) = 0;
};

#endif /* ICLIENTREPOSITORY_HPP */
