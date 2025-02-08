#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <string>
#include <vector>

class Parser {
public:
  Parser(std::string message = "");
  ~Parser();
  Parser(const Parser &other);
  Parser &operator=(const Parser &other);

  const std::string &getPrefix() { return this->_prefix; }
  const std::string &getCommand() { return this->_command; }
  const std::vector<std::string> &getParams() { return this->_params; }

private:
  std::string _prefix;
  std::string _command;
  std::vector<std::string> _params;
};

#endif /* PARSER_HPP */
