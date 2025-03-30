#include "domain/message/Message.hpp"

inline static MessageConstants::CommandType strToCommandType(const std::string &str);
inline static std::string enumToCommandStr(const MessageConstants::CommandType &command);
inline static std::string getPrefixString(const PrefixInfo &prefixInfo);

static const std::string CRLF = "\r\n";

std::ostream &operator<<(std::ostream &os, const Message &msg) {
  std::vector<std::string>::const_iterator it;
  if (msg.getPrefix() != "") {
    os << msg.getPrefix() << " ";
  }
  if (msg.isNumericResponse()) {
    os << std::setfill('0') << std::setw(3) << msg.getNumericResponse() << " ";
  } else {
    os << enumToCommandStr(msg.getCommand()) << " ";
  }
  for (it = msg.getParams().begin(); it != msg.getParams().end(); ++it) {
    if (it + 1 == msg.getParams().end() && msg.getIsIncludeTrailing()) {
      os << ":";
    }
    os << *it;
    if (it + 1 != msg.getParams().end()) {
      os << " ";
    }
  }
  os << CRLF;
  return os;
}

Message::Message() {
  this->_prefix = "";
  this->_command = MessageConstants::UNDEFINED;
  this->_params = std::vector<std::string>();
  this->_isNumericResponse = false;
  this->_numericResponse = 0;
  this->_isIncludeTrailing = 0;
}

Message::Message(const std::string &message) {
  *this = Message();
  if (parseMessage(message) != 0) {
    *this = Message();
    this->_command = MessageConstants::ERROR;
  }
}

Message::Message(
    const std::string prefix, MessageConstants::CommandType command, const std::string params) {
  if (prefix.length() == 0) {
    this->_prefix = "";
  } else {
    this->_prefix = ":" + prefix;
  }
  this->_command = command;
  parseParams(this->_params, params, &this->_isIncludeTrailing);
  this->_isNumericResponse = false;
  this->_numericResponse = 0;
}

Message::Message(const std::string prefix, const int responseCode, const std::string params) {
  if (prefix.length() == 0) {
    this->_prefix = "";
  } else {
    this->_prefix = ":" + prefix;
  }
  this->_command = MessageConstants::UNDEFINED;
  parseParams(this->_params, params, &this->_isIncludeTrailing);
  this->_numericResponse = responseCode;
  this->_isNumericResponse = true;
}

Message::~Message() {}

Message::Message(const Message &obj) { *this = obj; }

Message &Message::operator=(const Message &obj) {
  if (this != &obj) {
    this->_prefixObj = obj._prefixObj;
    this->_prefix = obj._prefix;
    this->_command = obj._command;
    this->_isNumericResponse = obj._isNumericResponse;
    this->_numericResponse = obj._numericResponse;
    this->_isIncludeTrailing = obj._isIncludeTrailing;
    this->_params = obj._params;
  }
  return *this;
}

const std::string &Message::getPrefix() const { return this->_prefix; }

const MessageConstants::CommandType &Message::getCommand() const { return this->_command; }

const std::vector<std::string> &Message::getParams() const { return this->_params; }

const int &Message::getNumericResponse() const { return this->_numericResponse; }

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
  std::ostringstream os;

  while (iss >> word) {
    if (word == CRLF) {
      break;
    }
    if (word[0] == ':' && words.size() > 0) {
      std::string remaining;
      getline(iss, remaining);
      if (!remaining.empty() && remaining[remaining.size() - 1] == '\r') {
        remaining.resize(remaining.size() - 1);
      }
      word += remaining;
      words.push_back(word);
      break;
    }
    words.push_back(word);
  }
  if (words.size() < 1) {
    return 1;
  }
  int error = 0;
  if (words[0][0] == ':') {
    if (this->parsePrefixDetails(this->_prefixObj, words[0]) != 0) {
      error |= 1;
    }
    this->_prefix = getPrefixString(this->_prefixObj);
    if (words.size() > 1 && parseCommand(this->_command, words[1]) != 0) {
      error |= 1;
    }
    if (words.size() > 1) {
      std::copy(words.begin() + 2, words.end(), std::ostream_iterator<std::string>(os, " "));
      size_t len = os.str().length();
      parseParams(this->_params, os.str().substr(0, len - 1), &this->_isIncludeTrailing);
    }
  } else {
    if (parseCommand(this->_command, words[0]) != 0) {
      error |= 1;
    }
    std::copy(words.begin() + 1, words.end(), std::ostream_iterator<std::string>(os, " "));
    size_t len = os.str().length();
    parseParams(this->_params, os.str().substr(0, len - 1), &this->_isIncludeTrailing);
  }
  return error;
}

int Message::parsePrefixDetails(PrefixInfo &prefixInfo, const std::string prefix) {
  std::string new_nick;
  std::string new_user;
  std::string new_host;

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
    new_nick = prefix.substr(0, userPos);
    if (hostPos != std::string::npos) {
      new_user = prefix.substr(userPos + 1, hostPos - userPos - 1);
      new_host = prefix.substr(hostPos + 1);
      if (new_user.length() == 0 || new_host.length() == 0) {
        return 1;
      }
    } else {
      new_user = prefix.substr(userPos + 1);
      if (new_user.length() == 0) {
        return 1;
      }
    }
  } else if (hostPos != std::string::npos) {
    new_nick = prefix.substr(0, hostPos);
    new_host = prefix.substr(hostPos + 1);
    if (new_host.length() == 0) {
      return 1;
    }
  } else {
    new_nick = prefix;
  }
  if (new_nick.length() == 0) {
    return 1;
  }
  prefixInfo.nick = new_nick;
  prefixInfo.user = new_user;
  prefixInfo.host = new_host;
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
    std::vector<std::string> &params, const std::string paramStr, int *const isIncludeTrailing) {
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
      if (!remaining.empty() && remaining[remaining.size() - 1] == '\r') {
        remaining.resize(remaining.size() - 1);
      }
      word += remaining;
      if (isIncludeTrailing != NULL)
        *isIncludeTrailing = 1;
      params.push_back(word.substr(1));
      break;
    }
    params.push_back(word);
  }
  return 0;
}

int Message::getIsIncludeTrailing() const { return this->_isIncludeTrailing; }

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

inline static std::string enumToCommandStr(const MessageConstants::CommandType &command) {
  switch (command) {
  case MessageConstants::PASS:
    return "PASS";
  case MessageConstants::NICK:
    return "NICK";
  case MessageConstants::USER:
    return "USER";
  case MessageConstants::JOIN:
    return "JOIN";
  case MessageConstants::PRIVMSG:
    return "PRIVMSG";
  case MessageConstants::KICK:
    return "KICK";
  case MessageConstants::INVITE:
    return "INVITE";
  case MessageConstants::TOPIC:
    return "TOPIC";
  case MessageConstants::MODE:
    return "MODE";
  case MessageConstants::ERROR:
    return "ERROR";
  default:
    return "UNKNOWN";
  }
}

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
