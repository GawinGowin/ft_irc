#ifndef BASEMESSAGE_HPP
#define BASEMESSAGE_HPP

#include "domain/message/IMessageAggregateRoot.hpp"
#include <string>
#include <vector>

class BaseMessage : public IMessageAggregateRoot {
public:
  BaseMessage();
  BaseMessage(const std::string &message);
  ~BaseMessage();
  BaseMessage(const BaseMessage &obj);
  BaseMessage &operator=(const BaseMessage &obj);

  const std::string &getPrefix() const;
  const CommandType &getCommand() const;
  const std::vector<std::string> &getParams() const;

private:
  std::string _prefix;
  CommandType _command;
  std::vector<std::string> _params;
};

#endif /* BASEMESSAGE_HPP */
