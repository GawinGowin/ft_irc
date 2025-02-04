#ifndef CONFIGS_HPP
#define CONFIGS_HPP

#include "domain/configs/ChannelConfig.hpp"
#include "domain/configs/GlobalConfig.hpp"
#include "domain/configs/LimitsConfig.hpp"
#include "domain/configs/OperatorConfig.hpp"
#include "domain/configs/OptionsConfig.hpp"
#include "domain/shared/values/IHashAggregateRoot.hpp"

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
  const OperatorConfig &getOperator() const;
  const ChannelConfig &getChannel() const;

  void setPort(const int port);
  void setPassword(const std::string password);

  GlobalConfig Global;
  LimitsConfig Limits;
  OptionsConfig Options;
  OperatorConfig Operator;
  ChannelConfig Channel;
};

#endif /* CONFIGS_HPP */
