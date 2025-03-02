#ifndef IMESSAGEAGGREGATEROOT_HPP
#define IMESSAGEAGGREGATEROOT_HPP

#include "domain/message/MessageConstants.hpp"
#include <string>
#include <vector>

class IMessageAggregateRoot {
public:
  virtual ~IMessageAggregateRoot(){};

  virtual const std::string &getPrefix() const = 0;
  virtual const MessageConstants::CommandType &getCommand() const = 0;
  virtual const std::vector<std::string> &getParams() const = 0;
};

#endif /* IMESSAGEAGGREGATEROOT_HPP */
