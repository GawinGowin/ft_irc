#ifndef SENDMSGDTO_HPP
#define SENDMSGDTO_HPP

#include "domain/message/Message.hpp"
#include "domain/message/MessageStreamVector.hpp"
#include <iostream>
#include <vector>

class SendMsgDTO {
public:
  SendMsgDTO();
  SendMsgDTO(int status, const MessageStreamVector &message);
  ~SendMsgDTO();
  SendMsgDTO(const SendMsgDTO &obj);
  SendMsgDTO &operator=(const SendMsgDTO &obj);

  const int &getStatus() const;
  void setStatus(const int status);

  const MessageStreamVector &getMessageStreams() const;
  void setMessageStreams(const MessageStreamVector &message);

private:
  int _status;
  MessageStreamVector _messageStreams;
};

#endif /* SENDMSGDTO_HPP */
