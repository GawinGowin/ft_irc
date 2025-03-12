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
const int RPL_INVITING = 341;
const int RPL_NAMREPLY = 353;
const int RPL_ENDOFNAMES = 366;
const int ERR_NEEDMOREPARAMS = 461;
const int ERR_ALREADYREGISTRED = 462;
const int ERR_NOSUCHNICK = 401;
const int RPL_CHANNELMODEIS = 324;
const int RPL_CREATIONTIME_MSG = 329;
const int ERR_CHANOPRIVSNEEDED = 482;
const int ERR_NOSUCHCHANNEL = 403;
const int ERR_CANNOTSENDTOCHAN = 404;
const int ERR_USERNOTINCHANNEL = 441;
const int ERR_NOTONCHANNEL = 442;
const int ERR_USERONCHANNEL = 443;
} // namespace ResponseCode
} // namespace MessageConstants

#endif /* MESSAGECONSTANTS_HPP */
