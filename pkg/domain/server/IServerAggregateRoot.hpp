#ifndef ISERVERAGGREGATEROOT_HPP
#define ISERVERAGGREGATEROOT_HPP

#include <string>
#include <vector>

class IServerAggregateRoot {
public:
  virtual ~IServerAggregateRoot() = 0;

  virtual bool isValidPassword(const std::string &password) = 0;

  virtual const std::string &getPasswordAsHash() const = 0;

  virtual const int &getServerSocket() const = 0;
  virtual const bool &isListening() const = 0;
};

#endif /* ISERVERAGGREGATEROOT_HPP */
