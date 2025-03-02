#ifndef MESSAGECONSTANTS_HPP
#define MESSAGECONSTANTS_HPP

#include <string>

namespace MessageConstants {
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

namespace ResponseCode {
const int RPL_WELCOME = 1;
const int RPL_YOURHOST = 2;
const int RPL_TOPIC = 332;
const int RPL_NAMREPLY = 353;
const int RPL_ENDOFNAMES = 366;
const int ERR_NEEDMOREPARAMS = 461;
const int ERR_ALREADYREGISTRED = 462;
} // namespace ResponseCode
} // namespace MessageConstants

#endif /* MESSAGECONSTANTS_HPP */
