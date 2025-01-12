#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "ChannelData.hpp"
#include "ChannelService.hpp"

class Client; // TODO: 仮での前方宣言/属しているユーザーリスト

class Channel {
public:

  const ChannelData getChannelData() const;
  const ChannelMode getChannelMode() const;

private:
  ChannelData _data;
  ChannelMode *_mode;
  std::list<Client*> _current_list;
  std::list<Client*> _invite_list;
};

#endif /* CHANNEL_HPP */
