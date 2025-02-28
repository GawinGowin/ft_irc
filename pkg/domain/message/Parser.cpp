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
    this->_command = IMessageAggregateRoot::UNKNOWN;
    return;
  }

  if (message[0] == ':') {
    size_t pos = message.find(' ');
    if (pos == std::string::npos) {
      this->_command = IMessageAggregateRoot::UNKNOWN;
      return;
    }
    this->_prefix = message.substr(1, pos - 1);
    message.erase(0, pos + 1);
  } else {
    this->_prefix = "";
  }
  eraseSpace(&message);
  if (message.length() == 2) {
    this->_command = IMessageAggregateRoot::UNKNOWN;
    return;
  }

  size_t pos = std::min(message.find(' '), message.find("\r\n"));
  std::string command = message.substr(0, pos);
  this->_command = this->strToEnum(command);
  if (command.length() == 3 && isdigit(command[0]) && isdigit(command[1]) && isdigit(command[2])) {
    this->_numberCommand = command;
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

int Parser::parsePrefixDetails(PrefixInfo &prefixInfo, const std::string &prefix) {
  if (prefix.length() == 0 || std::count(prefix.begin(), prefix.end(), '!') > 1 ||
      std::count(prefix.begin(), prefix.end(), '@') > 1) {
    return 1;
  }
  size_t userPos = prefix.find('!');
  size_t hostPos = prefix.find('@');
  if (userPos != std::string::npos && hostPos != std::string::npos && hostPos < userPos) {
    return 1;
  }
  if (userPos != std::string::npos) {
    prefixInfo.nick = prefix.substr(0, userPos);
    if (hostPos != std::string::npos) {
      prefixInfo.user = prefix.substr(userPos + 1, hostPos - userPos - 1);
      prefixInfo.host = prefix.substr(hostPos + 1);
      if (prefixInfo.user.length() == 0 || prefixInfo.host.length() == 0) {
        return 1;
      }
    } else {
      prefixInfo.user = prefix.substr(userPos + 1);
      if (prefixInfo.user.length() == 0) {
        return 1;
      }
    }
  } else if (hostPos != std::string::npos) {
    prefixInfo.nick = prefix.substr(0, hostPos);
    prefixInfo.host = prefix.substr(hostPos + 1);
    if (prefixInfo.host.length() == 0) {
      return 1;
    }
  } else {
    prefixInfo.nick = prefix;
  }
  if (prefixInfo.nick.length() == 0) {
    return 1;
  }
  return 0;
}

IMessageAggregateRoot::CommandType Parser::strToEnum(const std::string &str) {
  if (str == "PASS") {
    return (IMessageAggregateRoot::PASS);
  } else if (str == "NICK") {
    return (IMessageAggregateRoot::NICK);
  } else if (str == "USER") {
    return (IMessageAggregateRoot::USER);
  } else if (str == "JOIN") {
    return (IMessageAggregateRoot::JOIN);
  } else if (str == "PRIVMSG") {
    return (IMessageAggregateRoot::PRIVMSG);
  } else if (str == "KICK") {
    return (IMessageAggregateRoot::KICK);
  } else if (str == "INVITE") {
    return (IMessageAggregateRoot::INVITE);
  } else if (str == "TOPIC") {
    return (IMessageAggregateRoot::TOPIC);
  } else if (str == "MODE") {
    return (IMessageAggregateRoot::MODE);
  } else if (str == "ERROR") {
    return (IMessageAggregateRoot::ERROR);
  } else {
    return (IMessageAggregateRoot::UNKNOWN);
  }
}

const std::string Parser::enumToStr(const IMessageAggregateRoot::CommandType &type) {
  switch (type) {
  case IMessageAggregateRoot::PASS:
    return "PASS";
  case IMessageAggregateRoot::NICK:
    return "NICK";
  case IMessageAggregateRoot::USER:
    return "USER";
  case IMessageAggregateRoot::JOIN:
    return "JOIN";
  case IMessageAggregateRoot::PRIVMSG:
    return "PRIVMSG";
  case IMessageAggregateRoot::KICK:
    return "KICK";
  case IMessageAggregateRoot::INVITE:
    return "INVITE";
  case IMessageAggregateRoot::TOPIC:
    return "TOPIC";
  case IMessageAggregateRoot::MODE:
    return "MODE";
  case IMessageAggregateRoot::ERROR:
    return "ERROR";
  default:
    return "UNKNOWN";
  }
}
