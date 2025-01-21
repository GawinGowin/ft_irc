#ifndef INMEMORYCLIENTDATABASE_HPP
#define INMEMORYCLIENTDATABASE_HPP

#include "domain/client/IClientRepository.hpp"

#include <vector>
#include <stdexcept>

class InmemoryClientDatabase: virtual public IClientRepository {
    public:
      InmemoryClientDatabase();
      ~InmemoryClientDatabase();
      InmemoryClientDatabase(const InmemoryClientDatabase &other);
      InmemoryClientDatabase &operator=(const InmemoryClientDatabase &other);

      void addClient(const Client &client);
      const std::vector<Client> &getClients();
      const Client &getClientById(const int id);
      void popClientById(const int id);

    private:
      std::vector<Client> _clients;
};

#endif /* INMEMORYCLIENTDATABASE_HPP */
