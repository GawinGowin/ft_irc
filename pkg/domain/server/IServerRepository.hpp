#ifndef ISERVERREPOSITORY_HPP
#define ISERVERREPOSITORY_HPP

#include <string>

class IServerRepository {
public:
  virtual ~IServerRepository() = 0;
  virtual void setPassword(const std::string &password) = 0;
  virtual bool isValidPassword(const std::string &password) = 0;
};

#endif /* ISERVERREPOSITORY_HPP */
