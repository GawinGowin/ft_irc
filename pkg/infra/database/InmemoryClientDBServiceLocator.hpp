#ifndef INMEMORYINMEMORYCLIENTDBSERVICELOCATOR_HPP
#define INMEMORYINMEMORYCLIENTDBSERVICELOCATOR_HPP

#include "infra/database/InmemoryClientDatabase.hpp"

class InmemoryClientDBServiceLocator {
public:
  static InmemoryClientDatabase &get() {
    static InmemoryClientDatabase db;
    return db;
  }

private:
  InmemoryClientDBServiceLocator();
};

#endif /* INMEMORYINMEMORYCLIENTDBSERVICELOCATOR_HPP */
