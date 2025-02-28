#ifndef ICLIENTREPOSITORY_HPP
#define ICLIENTREPOSITORY_HPP

#include "domain/client/IClientAggregateRoot.hpp"
#include <vector>

class IClientRepository {
public:
  virtual ~IClientRepository() {};

  virtual void add(const IClientAggregateRoot &IClientAggregateRoot) = 0;
  virtual const std::vector<IClientAggregateRoot *> &list() = 0;
  virtual const std::vector<pollfd> &listPollfds() = 0;
  virtual IClientAggregateRoot *getById(const std::string &id) = 0;
  virtual void update(const std::string &id, const IClientAggregateRoot &newData) = 0;
  virtual void remove(const std::string &id) = 0;
};

#endif /* ICLIENTREPOSITORY_HPP */
