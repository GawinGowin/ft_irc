#ifndef JENKINSHASH_HPP
#define JENKINSHASH_HPP

#include "domain/shared/values/IHashAggregateRoot.hpp"
#include <sstream>
#include <string>

class JenkinsHash : virtual public IHashAggregateRoot {
public:
  JenkinsHash(const std::string &rawString);
  JenkinsHash(const JenkinsHash &obj);
  JenkinsHash &operator=(const JenkinsHash &obj);
  ~JenkinsHash();

  bool operator==(const IHashAggregateRoot &other) const;
  std::string getHash() const;
  long getHashLong() const;
  int getHashInt() const;

private:
  JenkinsHash();
  int _toHash(const std::string &src);
  int _hashedValue;
};

#endif /* JENKINSHASH_HPP */
