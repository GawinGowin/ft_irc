#ifndef CONFIGSLOADER_HPP
#define CONFIGSLOADER_HPP

#include "domain/configs/Configs.hpp"
#include "domain/configs/IConfigsRepository.hpp"

class ConfigsLoader : virtual public IConfigsRepository {
public:
  ConfigsLoader();
  ~ConfigsLoader();
  ConfigsLoader(const Configs &configs);
  ConfigsLoader &operator=(const ConfigsLoader &other);

  const Configs &getConfigs(void) const;
  void setConfigs(const Configs &configs);

private:
  Configs _configs;
};

#endif /* CONFIGSLOADER_HPP */
