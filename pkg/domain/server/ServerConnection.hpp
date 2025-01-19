#ifndef SERVERCONNECTION_HPP
#define SERVERCONNECTION_HPP

#include <arpa/inet.h>
#include <stdexcept>
#include <unistd.h>

class ServerConnection {
public:
  ServerConnection(const std::string &addr, const int &port);
  ~ServerConnection();

  const int &getWatchingAddress() const;
  const int &getClientSocket() const;
  const struct sockaddr_in &getAssignedAddress() const;

private:
  ServerConnection();
  ServerConnection(const ServerConnection &obj);
  ServerConnection &operator=(const ServerConnection &obj);

  int watchingAddress;
  int clientSocket;
  struct sockaddr_in assignedAddress;
};

#endif /* SERVERCONNECTION_HPP */
