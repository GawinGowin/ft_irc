#ifndef ISOCKETHANDLER_HPP
#define ISOCKETHANDLER_HPP

#include <poll.h>
#include <string>
#include <sys/types.h>

class ISocketHandler {
public:
  virtual ~ISocketHandler(){};

  virtual void initializeSocket() = 0;

  virtual void createPoll(pollfd *fds, nfds_t nfds) = 0;

  virtual int acceptConnection(struct sockaddr_in *clientAddr) = 0;
  virtual void closeConnection(int &targetSocket) = 0;
  virtual ssize_t sendMsg(const std::string &message, int &targetSocket) = 0;
  virtual std::string receiveMsg(const int &targetSocket) = 0;

  virtual const int &getServerSocket() const = 0;
  virtual void setMaxConnections(const int maxConnections) = 0;
  virtual void setMaxBufferSize(const int maxBufferSize) = 0;
  virtual const int &getPort() const = 0;
  virtual const int &getMaxConnections() const = 0;
  virtual const int &getMaxBufferSize() const = 0;
  virtual const bool &isListening() const = 0;
  virtual const pollfd &getServerPollfd() const = 0;

  virtual std::string getClientIp(const struct sockaddr_in &clientAddr) const = 0;
};

#endif /* ISOCKETHANDLER_HPP */
