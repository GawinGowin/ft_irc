#ifndef SERVERSERVICE_HPP
#define SERVERSERVICE_HPP

#include "IClientRepository.hpp"
#include "IServerServiceRepository.hpp"
#include "Server.hpp"

class ServerService : virtual public IServerServiceRepository {
public:
  static void registerClient(Server &svr, const IClientRepository &client);
  static void deleteClient(Server &svr, const IClientRepository &client);

private:
  ServerService();
  ~ServerService();
  ServerService(const ServerService &obj);
  ServerService &operator=(const ServerService &obj);
};

#endif /* SERVERSERVICE_HPP */
