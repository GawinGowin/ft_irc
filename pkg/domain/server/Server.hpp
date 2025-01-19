#ifndef SERVER_HPP
#define SERVER_HPP

#include "IServerRepository.hpp"
#include "ServerPassword.hpp"

#include <algorithm>
#include <vector>

class Server : virtual public IServerRepository {
public:
  Server(const std::string &addr, const int &port, const int &buf_size);
  ~Server();

  void setPassword(const std::string &password);
  bool isValidPassword(const std::string &password);

  void addClientById(const int &clientId);
  void removeClientById(const int &clientId);

private:
  Server();
  Server(const Server &obj);
  Server &operator=(const Server &obj);

  std::string _addr;
  int _port;
  int _buf_size;
  const ServerPassword *_srvPass;
  std::vector<int> _clientIds;
};

#endif /* SERVER_HPP */
