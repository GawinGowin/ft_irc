#ifndef ICONFIGSREPOSITORY_HPP
#define ICONFIGSREPOSITORY_HPP

#include "domain/configs/Configs.hpp"

class IConfigsRepository {
public:
  virtual ~IConfigsRepository() {};

  virtual const Configs &getConfigs(void) const = 0;
  virtual void setConfigs(const Configs &configs) = 0;
};

#endif /* ICONFIGSREPOSITORY_HPP */
