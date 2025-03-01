#include "domain/message/Message.hpp"

inline static MessageConstants::CommandType strToCommandType(const std::string &str);
inline static std::string getPrefixString(const PrefixInfo &prefixInfo);

static const std::string CRLF = "\r\n";

std::ostream &operator<<(std::ostream &os, const Message &msg) {
  std::vector<std::string>::const_iterator it;
  os << "{prefix: \"" << msg.getPrefix()
     << "\", "
        "command: "
     << msg.getCommand() << ", params: [";
  for (it = msg.getParams().begin(); it != msg.getParams().end(); ++it) {
    os << "\"" << (*it) << "\"";
    if (it + 1 != msg.getParams().end())
      os << ", ";
  }
  os << "]}";
  return os;
}

Message::Message() {
  this->_prefix = "";
  this->_command = MessageConstants::UNDEFINED;
  this->_params = std::vector<std::string>();
  this->_isNumericResponse = false;
  this->_numericResponse = "";
}

Message::Message(const std::string &message) {
  if (parseMessage(message) != 0) {
    this->_prefix = "";
    this->_command = MessageConstants::UNDEFINED;
    this->_params = std::vector<std::string>();
    this->_isNumericResponse = false;
    this->_numericResponse = "";
  }
}

Message::Message(
    const std::string prefix, MessageConstants::CommandType command, const std::string params) {
  this->_prefix = ":" + prefix;
  this->_command = command;
  parseParams(this->_params, params);
  this->_isNumericResponse = false;
  this->_numericResponse = "";
}

Message::Message(const std::string prefix, const int responseCode, const std::string params) {
  this->_prefix = ":" + prefix;
  this->_command = MessageConstants::UNDEFINED;
  parseParams(this->_params, params);

  std::ostringstream ss;
  ss << responseCode;
  this->_numericResponse = ss.str();
  this->_isNumericResponse = true;
}

Message::~Message() {}

Message::Message(const Message &obj) { *this = obj; }

Message &Message::operator=(const Message &obj) {
  if (this != &obj) {
    this->_prefix = obj._prefix;
    this->_command = obj._command;
    this->_params = obj._params;
  }
  return *this;
}

const std::string &Message::getPrefix() const { return this->_prefix; }

const MessageConstants::CommandType &Message::getCommand() const { return this->_command; }

const std::vector<std::string> &Message::getParams() const { return this->_params; }

const std::string &Message::getNumericResponse() const { return this->_numericResponse; }

const bool &Message::isNumericResponse() const { return this->_isNumericResponse; }

int Message::parseMessage(const std::string &msgStr) {
  /* RFC 2812に準拠し、メッセージの最大長は512バイト（CR+LFを含む） */
  std::string msg;
  if (msgStr.length() >= 510) {
    msg = msgStr.substr(0, 510) + CRLF;
  } else if (msgStr.length() < 2 || msgStr.substr(msgStr.length() - 2) != CRLF) {
    msg = msgStr + CRLF;
  } else {
    msg = msgStr;
  }

  std::istringstream iss(msg);
  std::string word;
  std::vector<std::string> words;

  while (iss >> word) {
    if (word == CRLF) {
      break;
    }
    if (word[0] == ':' && words.size() > 0) {
      std::string remaining;
      getline(iss, remaining);
      if (!remaining.empty() && remaining.back() == '\r') {
        remaining.pop_back();
      }
      word += remaining;
      words.push_back(word);
      break;
    }
    words.push_back(word);
  }

  if (words.size() < 2 && words.size() > 0) {
    return 1;
  }
  int error = 0;
  if (words[0][0] == ':') {
    if (this->parsePrefixDetails(this->_prefixObj, words[0]) != 0) {
      error |= 1;
    }
    this->_prefix = getPrefixString(this->_prefixObj);
    if (parseCommand(this->_command, words[1]) != 0) {
      error |= 1;
    }
    if (parseParams(this->_params, words.begin() + 2, words.end()) != 0) {
      error |= 1;
    }
  } else {
    if (parseCommand(this->_command, words[0]) != 0) {
      error |= 1;
    }
    if (parseParams(this->_params, words.begin() + 1, words.end()) != 0) {
      error |= 1;
    }
  }
  return error;
}

int Message::parsePrefixDetails(PrefixInfo &prefixInfo, const std::string prefix) {
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

int Message::parseCommand(MessageConstants::CommandType &command, const std::string message) {
  command = strToCommandType(message);
  if (command == MessageConstants::UNKNOWN) {
    return 1;
  }
  return 0;
}

int Message::parseParams(
    std::vector<std::string> &params,
    std::vector<std::string>::iterator paramIter,
    std::vector<std::string>::iterator end) {
  params.clear();
  while (paramIter != end) {
    params.push_back(*paramIter);
    paramIter++;
  }
  return 0;
}

int Message::parseParams(std::vector<std::string> &params, const std::string paramStr) {
  params.clear();
  std::istringstream iss(paramStr);
  std::string word;
  while (iss >> word) {
    if (word == CRLF) {
      break;
    }
    if (word[0] == ':') {
      std::string remaining;
      getline(iss, remaining);
      if (!remaining.empty() && remaining.back() == '\r') {
        remaining.pop_back();
      }
      word += remaining;
      params.push_back(word);
      break;
    }
    params.push_back(word);
  }
  return 0;
}

inline static MessageConstants::CommandType strToCommandType(const std::string &str) {
  if (str == "PASS") {
    return (MessageConstants::PASS);
  } else if (str == "NICK") {
    return (MessageConstants::NICK);
  } else if (str == "USER") {
    return (MessageConstants::USER);
  } else if (str == "JOIN") {
    return (MessageConstants::JOIN);
  } else if (str == "PRIVMSG") {
    return (MessageConstants::PRIVMSG);
  } else if (str == "KICK") {
    return (MessageConstants::KICK);
  } else if (str == "INVITE") {
    return (MessageConstants::INVITE);
  } else if (str == "TOPIC") {
    return (MessageConstants::TOPIC);
  } else if (str == "MODE") {
    return (MessageConstants::MODE);
  } else {
    return (MessageConstants::UNKNOWN);
  }
};

inline static std::string getPrefixString(const PrefixInfo &prefixInfo) {
  std::string prefix = "";
  if (prefixInfo.nick != "") {
    prefix += prefixInfo.nick;
    if (prefixInfo.user != "") {
      prefix += "!" + prefixInfo.user;
      if (prefixInfo.host != "") {
        prefix += "@" + prefixInfo.host;
      }
    } else if (prefixInfo.host != "") {
      prefix += "@" + prefixInfo.host;
    }
  }
  return prefix;
}
