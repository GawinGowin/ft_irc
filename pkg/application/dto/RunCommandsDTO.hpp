#ifndef RUNCOMMANDSDTO_HPP
#define RUNCOMMANDSDTO_HPP

#include "domain/message/IMessageAggregateRoot.hpp"
#include <stdexcept>
#include <string>

class RunCommandsDTO {
public:
  RunCommandsDTO() : _message(NULL) {}
  RunCommandsDTO(IMessageAggregateRoot *msgPt) : _message(msgPt) {}
  ~RunCommandsDTO() { delete this->_message; }
  RunCommandsDTO(const RunCommandsDTO &obj) { *this = obj; }
  RunCommandsDTO &operator=(const RunCommandsDTO &obj) {
    if (this != &obj) {
      this->_message = obj._message;
    }
    return *this;
  }
  const IMessageAggregateRoot &getMessage() const {
    if (this->_message == NULL) {
      throw std::invalid_argument("Message is empty");
    }
    return *_message;
  }

private:
  const IMessageAggregateRoot *_message;
};

#endif /* RUNCOMMANDSDTO_HPP */
