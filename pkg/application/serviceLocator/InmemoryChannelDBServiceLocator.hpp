#ifndef INMEMORYCHANNELDBSERVICELOCATOR_HPP
#define INMEMORYCHANNELDBSERVICELOCATOR_HPP

#include "infra/database/InmemoryChannelDatabase.hpp"

class InmemoryChannelDBServiceLocator {
public:
  static InmemoryChannelDatabase &get() {
    static InmemoryChannelDatabase db;
    return db;
  }

private:
  InmemoryChannelDBServiceLocator();
};

#endif /* INMEMORYCHANNELDBSERVICELOCATOR_HPP */
