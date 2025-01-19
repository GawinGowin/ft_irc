#ifndef ISERVERREPOSITORY_HPP
#define ISERVERREPOSITORY_HPP

#include <string>

class IServerRepository {
public:
  virtual ~IServerRepository() = 0;

  virtual void establishConnection() = 0;
  virtual void setParams(const std::string &addr, const int &port);
  virtual void setPassword(const std::string &password) = 0;

  virtual bool isValidPassword(const std::string &password) = 0;
};

#endif /* ISERVERREPOSITORY_HPP */
