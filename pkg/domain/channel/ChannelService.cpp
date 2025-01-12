    
#include "ChannelService.hpp"    

static const int max_name_length = 50;
static const CharsListSpecified forbiddenChars({":", ",", " ", "\a"});
static const CharsListSpecified channelNameInitialChars({"#", "&", "+", "!"});

bool ChannelService::isValidChannel(const ChannelData &channelData) {
  std::ostringstream oss;

  if (channelData.getName().empty()) {
    this->_errorMsg = "Channel name cannot be empty";
    return false;
  }
  if (channelData.getName().size() > max_name_length) {
    oss << "Channel name cannot be longer than " << max_name_length << " characters";
    this->_errorMsg = oss.str();
    return false;
  }
  std::list<std::string>::const_iterator it;
  for (it = channelNameInitialChars.begin(); it != channelNameInitialChars.end(); ++it) {
    if (channelData.getName()[0] == (*it)[0]) {
      break;
    }
    oss << "Channel name must start with " << channelNameInitialChars;
    this->_errorMsg = oss.str();
    return false;
  }
  for (it = forbiddenChars.begin(); it != forbiddenChars.end(); ++it) {
    if (channelData.getName().find(*it) != std::string::npos) {
      oss << "Channel name cannot contain " << forbiddenChars;
      this->_errorMsg = oss.str();
      return false;
    }
  }
  return true;
}

const std::string ChannelService::getErrorMsg() const {
  return this->_errorMsg;
}

std::ostream &operator<<(std::ostream &os, const CharsListSpecified &lst) {
  if (lst.empty()) {
    return os;
  }
  std::list<std::string>::const_iterator it = lst.begin();
  os << "'" << *it << "'";
  ++it;

  for (; it != lst.end(); ++it) {
    if (std::distance(it, lst.end()) == 1) {
      os << " or ";
    } else {
      os << ", ";
    }
    os << "'" << *it << "'";
  }
  return os;
}