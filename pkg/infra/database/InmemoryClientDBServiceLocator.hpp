#ifndef INMEMORYCLIENTDBSERVICELOCATOR_HPP
#define INMEMORYCLIENTDBSERVICELOCATOR_HPP

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

#endif /* INMEMORYCLIENTDBSERVICELOCATOR_HPP */
