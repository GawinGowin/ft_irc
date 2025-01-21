#ifndef CLIENTDBSERVICELOCATOR_HPP
#define CLIENTDBSERVICELOCATOR_HPP

#include "infra/database/InmemoryClientDatabase.hpp"

class ClientDBServiceLocator {
public:
    static InmemoryClientDatabase& get() {
        static InmemoryClientDatabase db;
        return db;
    }

private:
    ClientDBServiceLocator();
};

#endif /* CLIENTDBSERVICELOCATOR_HPP */
