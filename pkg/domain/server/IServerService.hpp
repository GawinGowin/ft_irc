#ifndef ISERVERSERVICE_HPP
#define ISERVERSERVICE_HPP

#include "IClientRepository.hpp"
#include "IServerRepository.hpp"

class IServerService {
public:
  virtual void registerClient(IServerRepository &svr, const IClientRepository &client) = 0;
  virtual void deleteClient(IServerRepository &svr, const IClientRepository &client) = 0;
};

#endif /* ISERVERSERVICE_HPP */
