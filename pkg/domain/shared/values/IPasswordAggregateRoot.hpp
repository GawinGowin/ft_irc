#ifndef IPASSWORDAGGREGATEROOT_HPP
#define IPASSWORDAGGREGATEROOT_HPP

#include <string>
#include <vector>

class IPasswordAggregateRoot {
public:
  virtual ~IPasswordAggregateRoot(){};

  virtual std::string getHash() const = 0;
};

#endif /* IPASSWORDAGGREGATEROOT_HPP */
