#ifndef ISERVERSERVICEREPOSITORY_HPP
#define ISERVERSERVICEREPOSITORY_HPP

class IServerServiceRepository {
public:
  virtual void registerClientById(const int &clientId) = 0;
  virtual void deleteClientById(const int &clientId) = 0;
};

#endif /* ISERVERSERVICEREPOSITORY_HPP */
