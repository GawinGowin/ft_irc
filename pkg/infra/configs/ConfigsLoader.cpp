#include "infra/configs/ConfigsLoader.hpp"

ConfigsLoader::ConfigsLoader() {}

ConfigsLoader::~ConfigsLoader() {}

ConfigsLoader::ConfigsLoader(const Configs &other) { *this = other; }

ConfigsLoader &ConfigsLoader::operator=(const ConfigsLoader &other) {
  if (this != &other) {
    this->_configs = other._configs;
  }
  return *this;
}

const Configs &ConfigsLoader::getConfigs(void) const { return this->_configs; }

void ConfigsLoader::setConfigs(const Configs &configs) { this->_configs = configs; }
