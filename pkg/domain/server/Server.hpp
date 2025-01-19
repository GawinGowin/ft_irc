#ifndef SERVER_HPP
#define SERVER_HPP

#include "IServerRepository.hpp"
#include "ServerConnection.hpp"
#include "ServerPassword.hpp"

#include <algorithm>
#include <vector>

class Server : virtual public IServerRepository {
public:
  // Server(const std::string &addr, const int &port, const int &buf_size);
  Server();
  ~Server();

  void establishConnection();
  void setParams(const std::string &addr, const int &port);
  void setPassword(const std::string &password);
  bool isValidPassword(const std::string &password);

  void registerClientById(const int &clientId);
  void deleteClientById(const int &clientId);

  const std::vector<int> &getAcceptedClients() const { return _clientFds; };

private:
  std::string _addr;
  int _port;
  // int _buf_size;
  const ServerPassword *_srvPass;
  ServerConnection *_srvConn;
  std::vector<int> _clientFds;

  Server(const Server &obj);
  Server &operator=(const Server &obj);
};

#endif /* SERVER_HPP */
