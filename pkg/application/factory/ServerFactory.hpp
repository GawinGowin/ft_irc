#ifndef SERVERFACTORY_HPP
#define SERVERFACTORY_HPP

#include "Server.hpp"
#include "ServerService.hpp"

class ServerFactory {
  public:
    static Server *createServer() {
        return new Server();
    }
    static ServerService *createServerService() {
        return new ServerService();
    }
};


#endif /* SERVERFACTORY_HPP */
