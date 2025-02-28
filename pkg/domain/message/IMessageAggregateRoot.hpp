#ifndef IMESSAGEAGGREGATEROOT_HPP
#define IMESSAGEAGGREGATEROOT_HPP

#include <string>
#include <vector>

class IMessageAggregateRoot {
public:
  enum CommandType {
    PASS,
    NICK,
    USER,
    JOIN,
    PRIVMSG,
    KICK,
    INVITE,
    TOPIC,
    MODE,
    ERROR,
    UNKNOWN,
    UNDEFINED
  };

  virtual ~IMessageAggregateRoot() {};

  virtual const std::string &getPrefix() const = 0;
  virtual const CommandType &getCommand() const = 0;
  virtual const std::vector<std::string> &getParams() const = 0;
};

#endif /* IMESSAGEAGGREGATEROOT_HPP */
