#include "Parser.hpp"

static void eraseSpace(std::string *str) {
  size_t cnt = 0;
  while (cnt < str->length() && (*str)[cnt] == ' ') {
    ++cnt;
  }
  if (cnt > 0)
    str->erase(0, cnt);
}

Parser::Parser(std::string message) {
  if (message.length() >= 510) {
    message = message.substr(0, 510) + "\r\n";
  }
  eraseSpace(&message);

  if (message.length() < 2 || message.substr(message.length() - 2) != "\r\n") {
    throw std::runtime_error("Parse error: Does not end with CRLF");
  }

  if (message[0] == ':') {
    size_t pos = message.find(' ');
    if (pos == std::string::npos) {
      throw std::runtime_error("Parse error: Command is required");
    }
    this->_prefix = message.substr(1, pos - 1);
    message.erase(0, pos + 1);
  } else {
    this->_prefix = "";
  }
  eraseSpace(&message);
  if (message.length() == 2) {
    throw std::runtime_error("Parse error: Command is required");
  }

  size_t pos = std::min(message.find(' '), message.find("\r\n"));
  std::string command = message.substr(0, pos);
  if (command == "PASS") {
    this->_command = IMessageAggregateRoot::PASS;
  } else if (command == "NICK") {
    this->_command = IMessageAggregateRoot::NICK;
  } else if (command == "USER") {
    this->_command = IMessageAggregateRoot::USER;
  } else if (command == "JOIN") {
    this->_command = IMessageAggregateRoot::JOIN;
  } else if (command == "PRIVMSG") {
    this->_command = IMessageAggregateRoot::PRIVMSG;
  } else if (command == "KICK") {
    this->_command = IMessageAggregateRoot::KICK;
  } else if (command == "INVITE") {
    this->_command = IMessageAggregateRoot::INVITE;
  } else if (command == "TOPIC") {
    this->_command = IMessageAggregateRoot::TOPIC;
  } else if (command == "MODE") {
    this->_command = IMessageAggregateRoot::MODE;
  } else if (command == "ERROR") {
    this->_command = IMessageAggregateRoot::ERROR;
  } else {
    this->_command = IMessageAggregateRoot::UNKNOWN;
  }
  message.erase(0, pos);
  if (message.length() == 2) {
    return;
  }
  eraseSpace(&message);
  while (message.length() > 2) {
    if (message[0] == ':') {
      this->_params.push_back(message.substr(1, message.length() - 3));
      break;
    }
    size_t pos = message.find(' ');
    if (pos == std::string::npos) {
      this->_params.push_back(message.substr(0, message.length() - 2));
      break;
    }
    this->_params.push_back(message.substr(0, pos));
    message.erase(0, pos);
    eraseSpace(&message);
  }
}
