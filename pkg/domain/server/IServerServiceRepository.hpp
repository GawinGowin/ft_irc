#ifndef ISERVERSERVICEREPOSITORY_HPP
#define ISERVERSERVICEREPOSITORY_HPP

class IServerServiceRepository {
public:
  virtual void addClientById(const int &clientId) = 0;
  virtual void removeClientById(const int &clientId) = 0;
};

#endif /* ISERVERSERVICEREPOSITORY_HPP */
