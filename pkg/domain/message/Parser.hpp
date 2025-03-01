#ifndef PARSER_HPP
#define PARSER_HPP

#include "IMessageAggregateRoot.hpp"
#include "domain/message/MessageConstants.hpp"
#include "domain/message/PrefixInfo.hpp"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

class Parser : public IMessageAggregateRoot {
public:
  Parser(std::string message = "");
  ~Parser(){};
  const std::string &getPrefix() const { return this->_prefix; }
  const MessageConstants::CommandType &getCommand() const { return this->_command; }
  const std::vector<std::string> &getParams() const { return this->_params; }

  static int parsePrefixDetails(PrefixInfo &prefixInfo, const std::string &prefix);
  static MessageConstants::CommandType strToEnum(const std::string &str);

private:
  std::string _prefix;
  MessageConstants::CommandType _command;
  std::vector<std::string> _params;

  std::string _numberCommand;
};

#endif /* PARSER_HPP */
