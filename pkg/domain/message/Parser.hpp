#ifndef PARSER_HPP
#define PARSER_HPP

#include "IMessageAggregateRoot.hpp"
#include <iostream>
#include <string>
#include <vector>

class Parser : public IMessageAggregateRoot {
public:
  Parser(std::string message = "");
  ~Parser(){};
  const std::string &getPrefix() const { return this->_prefix; }
  const CommandType &getCommand() const { return this->_command; }
  const std::vector<std::string> &getParams() const { return this->_params; }

private:
  std::string _prefix;
  CommandType _command;
  std::vector<std::string> _params;
};

#endif /* PARSER_HPP */
