#ifndef SENDMSGDTO_HPP
#define SENDMSGDTO_HPP

#include "domain/message/Message.hpp"
#include <iostream>

class SendMsgDTO {
public:
  SendMsgDTO();
  SendMsgDTO(const Message &message);
  ~SendMsgDTO();
  SendMsgDTO(const SendMsgDTO &obj);
  SendMsgDTO &operator=(const SendMsgDTO &obj);

  const Message &getMessage() const;
  const int &getStatus() const;

  void setMessage(const Message &message);
  void setStatus(const int status);

private:
  Message _message;
  int _status;
};

std::ostream &operator<<(std::ostream &os, const SendMsgDTO &msg);

#endif /* SENDMSGDTO_HPP */
