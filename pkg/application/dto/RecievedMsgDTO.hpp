#ifndef RECIEVEDMSGDTO_HPP
#define RECIEVEDMSGDTO_HPP

#include "domain/client/IClientAggregateRoot.hpp"
#include <string>

class RecievedMsgDTO {
public:
  RecievedMsgDTO();
  RecievedMsgDTO(const std::string &message, IClientAggregateRoot *client);
  ~RecievedMsgDTO(){};
  RecievedMsgDTO(const RecievedMsgDTO &obj);
  RecievedMsgDTO &operator=(const RecievedMsgDTO &obj);

  const std::string &getMessage() const;
  const int &getSenderFd() const;
  IClientAggregateRoot *getClient() const;

private:
  std::string _message;
  int _senderFd;
  IClientAggregateRoot *_client;
};

#endif /* RECIEVEDMSGDTO_HPP */
