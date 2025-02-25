#ifndef NICK_HPP
#define NICK_HPP

#include "application/commands/ACommands.hpp"

class Nick : public ACommands {
public:
  Nick();
  Nick(IMessageAggregateRoot *msg, IClientAggregateRoot *client);
  ~Nick();
  Nick(const Nick &obj);
  Nick &operator=(const Nick &obj);

  SendMsgDTO execute();

private:
  void _setNickName(std::string nick);
};

#endif /* NICK_HPP */
