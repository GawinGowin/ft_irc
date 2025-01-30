#ifndef IHASHAGGREGATEROOT_HPP
#define IHASHAGGREGATEROOT_HPP

#include <string>

class IHashAggregateRoot {
public:
  virtual ~IHashAggregateRoot(){};
  virtual bool operator==(const IHashAggregateRoot &other) const = 0;

  virtual std::string getHash() const = 0;
  virtual long getHashLong() const = 0;
  virtual int getHashInt() const = 0;
};

#endif /* IHASHAGGREGATEROOT_HPP */
