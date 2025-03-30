#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "domain/message/IMessageAggregateRoot.hpp"
#include "domain/message/MessageConstants.hpp"
#include "domain/message/PrefixInfo.hpp"
#include <algorithm>
#include <iomanip>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

class Message : public IMessageAggregateRoot {
public:
  Message();
  Message(const std::string &message);
  Message(
      const std::string prefix, MessageConstants::CommandType command, const std::string params);
  Message(const std::string prefix, const int responseCode, const std::string params);

  ~Message();
  Message(const Message &obj);
  Message &operator=(const Message &obj);

  const std::string &getPrefix() const;
  const MessageConstants::CommandType &getCommand() const;
  const std::vector<std::string> &getParams() const;
  const int &getNumericResponse() const;
  const bool &isNumericResponse() const;

  int parseMessage(const std::string &message);
  static int parsePrefixDetails(PrefixInfo &prefixInfo, const std::string prefix);
  static int parseCommand(MessageConstants::CommandType &command, const std::string message);
  static int parseParams(
      std::vector<std::string> &params, const std::string paramStr, int *const isIncludeTrailing);

  int getIsIncludeTrailing() const;

private:
  PrefixInfo _prefixObj;
  std::string _prefix;

  MessageConstants::CommandType _command;
  bool _isNumericResponse;
  int _numericResponse;
  int _isIncludeTrailing;
  std::vector<std::string> _params;
};

std::ostream &operator<<(std::ostream &os, const Message &msg);

#endif /* MESSAGE_HPP */
