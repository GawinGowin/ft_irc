#ifndef CONFIGSLOADER_HPP
#define CONFIGSLOADER_HPP

#include "domain/configs/Configs.hpp"
#include "domain/configs/IConfigsRepository.hpp"
#include <ctime>
#include <sstream>


class ConfigsLoader : virtual public IConfigsRepository {
public:
  ConfigsLoader();
  ~ConfigsLoader();
  ConfigsLoader(const Configs &configs);
  ConfigsLoader &operator=(const ConfigsLoader &other);

  const Configs &getConfigs(void) const;
  void setConfigs(const Configs &configs);

  void setPort(const int &port);
  void setPassword(const std::string &password);
  void setStartStr();

private:
  Configs _configs;
};

#endif /* CONFIGSLOADER_HPP */
