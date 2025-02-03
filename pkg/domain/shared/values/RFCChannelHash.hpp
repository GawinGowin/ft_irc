#ifndef RFCCHANNELHASH_HPP
#define RFCCHANNELHASH_HPP

#include "domain/shared/values/IHashAggregateRoot.hpp"
#include <cmath>
#include <ctime>
#include <sstream>
#include <string>

class RFCChannelHash : virtual public IHashAggregateRoot {
public:
  RFCChannelHash();
  RFCChannelHash(const std::time_t &current_time);
  RFCChannelHash(const RFCChannelHash &obj);
  RFCChannelHash &operator=(const RFCChannelHash &obj);
  ~RFCChannelHash();

  bool operator==(const IHashAggregateRoot &other) const;
  bool operator<(const IHashAggregateRoot &other) const;

  std::string getHash() const;
  long getHashLong() const;
  int getHashInt() const;

private:
  int _toHash(const std::time_t &current_time) const;
  long _hashedValue;
};

#endif /* RFCCHANNELHASH_HPP */
