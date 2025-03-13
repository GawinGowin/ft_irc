#include "domain/channel/Channel.hpp"

Channel::Channel(const std::string &name) {
  time_t now = time(NULL);
  this->_name = name;
  this->_id = JenkinsHash(name);
  this->_modeFlags = 0;
  this->_creation_time = now;
  this->_topic = ChannelTopic();
  this->_key = "";
  this->_maxusers = DEFAULT_MAX_USERS;
  this->_listConnects = ChannelClientList();
  this->_listBans = ChannelClientList();
  this->_listExcepts = ChannelClientList();
  this->_listInvites = ChannelClientList();
}

Channel::~Channel() {}

const std::string &Channel::getName() const { return this->_name; }

const JenkinsHash &Channel::getId() const { return this->_id; }

const int &Channel::getModeFlags() const { return this->_modeFlags; }

const time_t &Channel::getCreationTime() const { return this->_creation_time; }

const std::string &Channel::getTopic() const { return this->_topic.getTopic(); }

const std::string Channel::getMaxUsers() const {
  std::stringstream ss;
  ss << this->_maxusers;
  return ss.str();
}

const std::string &Channel::getKey() const { return this->_key; }

void Channel::setKey(const std::string key) { this->_key = key; }

void Channel::setModeFlags(int modeFlags) { this->_modeFlags = modeFlags; }

void Channel::setMaxUsers(unsigned long maxusers) { this->_maxusers = maxusers; }

Channel *Channel::clone() const { return new Channel(*this); }

ChannelClientList &Channel::getListConnects() { return this->_listConnects; }

ChannelClientList &Channel::getListBans() { return this->_listBans; }

ChannelClientList &Channel::getListExcepts() { return this->_listExcepts; }

ChannelClientList &Channel::getListInvites() { return this->_listInvites; }

bool Channel::isKeyProtected() const {
  return (this->_modeFlags & MODE_KEY_PROTECTED) && !this->_key.empty();
}

bool Channel::checkKey(const std::string &key) const {
  if (!this->isKeyProtected()) {
    return true;
  }
  return this->_key == key;
}

bool Channel::isMemberLimitExceeded() {
  if (!(this->_modeFlags & MODE_LIMIT_USERS)) {
    return false;
  }
  ChannelClientList &listConnects = this->_listConnects;
  return listConnects.getClients().size() >= this->_maxusers;
}

bool Channel::isInviteOnly() const { return (this->_modeFlags & MODE_INVITE_ONLY); }

bool Channel::isUserInvited(const std::string &nickname) {
  return this->_listInvites.isClientInList(nickname);
}

bool Channel::isOperator(const std::string &nickname) const {
  return this->_listOperators.isClientInList(nickname);
}

void Channel::addOperator(const std::string &nickname) { this->_listOperators.addClient(nickname); }

void Channel::removeOperator(const std::string &nickname) {
  this->_listOperators.removeClient(nickname);
}
