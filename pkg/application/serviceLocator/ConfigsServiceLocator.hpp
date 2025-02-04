#ifndef CONFIGSSERVICELOCATOR_HPP
#define CONFIGSSERVICELOCATOR_HPP

#include "infra/configs/ConfigsLoader.hpp"

class ConfigsServiceLocator {
public:
  static ConfigsLoader &get() {
    static ConfigsLoader conf;
    return conf;
  }

private:
  ConfigsServiceLocator();
};

#endif /* CONFIGSSERVICELOCATOR_HPP */
