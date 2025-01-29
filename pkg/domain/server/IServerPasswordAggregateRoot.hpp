#ifndef ISERVERPASSWORDAGGREGATEROOT_HPP
#define ISERVERPASSWORDAGGREGATEROOT_HPP

#include <string>
#include <vector>

class IServerPasswordAggregateRoot {
public:
  virtual ~IServerPasswordAggregateRoot() {};

  virtual std::string getHash() const = 0;
};

#endif /* ISERVERPASSWORDAGGREGATEROOT_HPP */
