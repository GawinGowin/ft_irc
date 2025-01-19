#ifndef SERVERFACTORY_HPP
#define SERVERFACTORY_HPP

#include "Server.hpp"

class ServerFactory {
  public:
    static Server *createServer() {
        return new Server();
    }
};


#endif /* SERVERFACTORY_HPP */
