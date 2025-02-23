#ifndef SENDMSGDTO_HPP
#define SENDMSGDTO_HPP

#include "domain/message/Message.hpp"
#include <iostream>

class SendMsgDTO {
  public:
  SendMsgDTO(const Message &message);
  ~SendMsgDTO();
  SendMsgDTO(const SendMsgDTO &obj);
  SendMsgDTO &operator=(const SendMsgDTO &obj);
  
  const Message &getMessage() const;
  
  private:
  Message _message;
};

std::ostream &operator<<(std::ostream &os, const SendMsgDTO &msg);

#endif /* SENDMSGDTO_HPP */
