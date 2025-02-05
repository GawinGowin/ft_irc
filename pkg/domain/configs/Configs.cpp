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
    this->Operators = obj.Operators;
    this->Channels = obj.Channels;
  }
  return *this;
}

const GlobalConfig &Configs::getGlobal() const { return this->Global; }

const LimitsConfig &Configs::getLimits() const { return this->Limits; }

const OptionsConfig &Configs::getOptions() const { return this->Options; }

const std::vector<OperatorConfig> &Configs::getOperators() const { return this->Operators; }

const std::vector<ChannelConfig> &Configs::getChannels() const { return this->Channels; }
