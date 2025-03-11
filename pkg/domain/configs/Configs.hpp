#ifndef CONFIGS_HPP
#define CONFIGS_HPP

#include "domain/configs/GlobalConfig.hpp"
#include "domain/configs/LimitsConfig.hpp"
#include "domain/configs/OptionsConfig.hpp"

#include <string>
#include <vector>

struct Configs {
  Configs();
  ~Configs();
  Configs(const Configs &obj);
  Configs &operator=(const Configs &obj);

  const GlobalConfig &getGlobal() const;
  const LimitsConfig &getLimits() const;
  const OptionsConfig &getOptions() const;

  GlobalConfig Global;
  LimitsConfig Limits;
  OptionsConfig Options;
};

#endif /* CONFIGS_HPP */
