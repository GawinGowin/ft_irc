#include "domain/configs/Configs.hpp"

// Set the default values for the Configs struct
Configs::Configs() {}

Configs::~Configs() {}

Configs::Configs(const Configs &obj) { *this = obj; }

Configs &Configs::operator=(const Configs &obj) {
  if (this != &obj) {
    this->Global = obj.Global;
    this->Limits = obj.Limits;
    this->Options = obj.Options;
    this->Operator = obj.Operator;
    this->Channel = obj.Channel;
  }
  return *this;
}

const GlobalConfig &Configs::getGlobal() const {
  return this->Global;
}

const LimitsConfig &Configs::getLimits() const {
  return this->Limits;
}

const OptionsConfig &Configs::getOptions() const {
  return this->Options;
}

const OperatorConfig &Configs::getOperator() const {
  return this->Operator;
}

const ChannelConfig &Configs::getChannel() const {
  return this->Channel;
}

void Configs::setPort(const int port) {
  this->Global.Port = port;
}

void Configs::setPassword(const std::string password) {
  this->Global.Password = password;
}
