#ifndef RECIEVEDMSGDTO_HPP
#define RECIEVEDMSGDTO_HPP

#include <string>

class RecievedMsgDTO {
public:
  RecievedMsgDTO();
  RecievedMsgDTO(const std::string &message, const int &senderId);
  ~RecievedMsgDTO() {};
  RecievedMsgDTO(const RecievedMsgDTO &obj);
  RecievedMsgDTO &operator=(const RecievedMsgDTO &obj);

  const std::string &getMessage() const;
  const int &getSenderId() const;

private:
  std::string _message;
  int _senderId;
};

#endif /* RECIEVEDMSGDTO_HPP */