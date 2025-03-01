#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "domain/message/IMessageAggregateRoot.hpp"
#include "domain/message/MessageConstants.hpp"
#include "domain/message/Parser.hpp"
#include <sstream>
#include <string>
#include <vector>

class Message : public IMessageAggregateRoot {
public:
  Message();
  Message(const std::string &message);
  Message(
      const std::string prefix,
      MessageConstants::CommandType command,
      const std::vector<std::string> params);
  Message(const std::string prefix, const int responseCode, const std::vector<std::string> params);

  ~Message();
  Message(const Message &obj);
  Message &operator=(const Message &obj);

  const std::string &getPrefix() const;
  const MessageConstants::CommandType &getCommand() const;
  const std::vector<std::string> &getParams() const;
  const std::string &getNumericResponse() const;
  const bool &isNumericResponse() const;

private:
  std::string _prefix;
  MessageConstants::CommandType _command;
  std::vector<std::string> _params;

  bool _isNumericResponse;
  std::string _numericResponse;
};

std::ostream &operator<<(std::ostream &os, const Message &msg);

#endif /* MESSAGE_HPP */
