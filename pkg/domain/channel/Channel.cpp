#include "domain/channel/Channel.hpp"

Channel::Channel(const std::string &name) {
  time_t now = time(NULL);
  this->_name = name;
  this->_id = ChannelId(now);
  this->_modeFlags = 0;
  this->_creation_time = now;
  this->_topic = ChannelTopic();
  this->_key = NULL;
  this->_maxusers = DEFAULT_MAX_USERS;
  this->_listConnects = ChannelClientList();
  this->_listBans = ChannelClientList();
  this->_listExcepts = ChannelClientList();
  this->_listInvites = ChannelClientList();
}

Channel::~Channel() {
  if (this->_key != NULL) {
    delete this->_key;
    this->_key = NULL;
  }
}

const std::string &Channel::getName() const { return this->_name; }

const ChannelId &Channel::getId() const { return this->_id; }

const int &Channel::getModeFlags() const { return this->_modeFlags; }

const time_t &Channel::getCreationTime() const { return this->_creation_time; }

const ChannelTopic &Channel::getTopic() const { return this->_topic; }

const unsigned long &Channel::getMaxUsers() const { return this->_maxusers; }

IPasswordAggregateRoot *Channel::getKey() const { return this->_key; }

void Channel::setModeFlags(int modeFlags) { this->_modeFlags = modeFlags; }

void Channel::setMaxUsers(unsigned long maxusers) { this->_maxusers = maxusers; }

Channel *Channel::clone() const { return new Channel(*this); }

ChannelClientList &Channel::getListConnects() { return this->_listConnects; }

ChannelClientList &Channel::getListBans() { return this->_listBans; }

ChannelClientList &Channel::getListExcepts() { return this->_listExcepts; }

ChannelClientList &Channel::getListInvites() { return this->_listInvites; }
