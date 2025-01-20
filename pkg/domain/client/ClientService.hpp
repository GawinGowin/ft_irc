#ifndef CLIENTSERVICE_HPP
#define CLIENTSERVICE_HPP

#include "IClientServiceRepository.hpp"

class ClientService: virtual public IClientServiceRepository {
  public:
    ~ClientService();
};

#endif /* CLIENTSERVICE_HPP */
