#ifndef ICLIENTDATABASEREPOSITORY_HPP
#define ICLIENTDATABASEREPOSITORY_HPP

#include "domain/client/Client.hpp"
#include <vector>

class IClientDatabaseRepository {
  public:
    virtual ~IClientDatabaseRepository() = 0;
    virtual void addClient(const Client &client) = 0;
    virtual const std::vector<Client> &getClients() = 0;
    virtual const Client &getClientById(const int id) = 0;
    virtual void popClientById(const int id) = 0;
};

#endif /* ICLIENTDATABASEREPOSITORY_HPP */
