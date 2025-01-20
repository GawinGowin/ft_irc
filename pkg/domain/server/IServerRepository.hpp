#ifndef ISERVERREPOSITORY_HPP
#define ISERVERREPOSITORY_HPP

#include <string>
#include <vector>

class IServerRepository {
public:
  virtual ~IServerRepository() = 0;

  virtual void establishConnection() = 0;
  virtual void setParams(const std::string &addr, const int &port);
  virtual void setPassword(const std::string &password) = 0;

  virtual void registerClientById(const int &clientId) = 0;
  virtual void deleteClientById(const int &clientId) = 0;

  virtual bool isValidPassword(const std::string &password) = 0;
  virtual const std::vector<int> &getAcceptedClients() const = 0;
  virtual const int &getWatchingAddress() const = 0;
  virtual const int &getClientSocket() const = 0;
  virtual const struct sockaddr_in &getAssignedAddress() const = 0;
};

#endif /* ISERVERREPOSITORY_HPP */
