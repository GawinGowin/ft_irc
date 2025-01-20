#ifndef SERVERSERVICE_HPP
#define SERVERSERVICE_HPP

#include "IClientRepository.hpp"
#include "IServerService.hpp"
#include "IServerService.hpp"
#include "Server.hpp"
#include <stdexcept>

class ServerService : virtual public IServerService {
public:
  ServerService();
  void registerClient(IServerRepository &svr, const IClientRepository &client);
  void deleteClient(IServerRepository &svr, const IClientRepository &client);

private:
  ~ServerService();
  ServerService(const ServerService &obj);
  ServerService &operator=(const ServerService &obj);
};

#endif /* SERVERSERVICE_HPP */
