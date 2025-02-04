#ifndef OPERATORCONFIG_HPP
#define OPERATORCONFIG_HPP

#include <string>

struct OperatorConfig {
  std::string Name;
  std::string Password;
  std::string Mask;

  OperatorConfig() {}
  OperatorConfig(const std::string &Name, const std::string &Password, const std::string &Mask) {
    this->Name = Name;
    this->Password = Password;
    this->Mask = Mask;
  }
};

#endif /* OPERATORCONFIG_HPP */
