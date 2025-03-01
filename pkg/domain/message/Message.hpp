#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "domain/message/IMessageAggregateRoot.hpp"
#include <string>
#include <vector>

class Message : public IMessageAggregateRoot {
public:
  Message();
  Message(const std::string &message);
  ~Message();
  Message(const Message &obj);
  Message &operator=(const Message &obj);

  const std::string &getPrefix() const;
  const MessageConstants::CommandType &getCommand() const;
  const std::vector<std::string> &getParams() const;

private:
  std::string _prefix;
  MessageConstants::CommandType _command;
  std::vector<std::string> _params;
};

std::ostream &operator<<(std::ostream &os, const Message &msg);

#endif /* MESSAGE_HPP */
