#ifndef SHAHASH_HPP
#define SHAHASH_HPP

#include "domain/shared/values/IHashAggregateRoot.hpp"
#include <iomanip>
#include <iostream>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <sstream>
#include <string>

class SHAHash : virtual public IHashAggregateRoot {
public:
  SHAHash();
  SHAHash(const std::string &rawPassword);
  SHAHash(const SHAHash &obj);
  ~SHAHash();
  SHAHash &operator=(const SHAHash &obj);

  bool operator==(const IHashAggregateRoot &other) const;
  bool operator<(const IHashAggregateRoot &other) const;

  std::string getHash() const;
  long getHashLong() const;
  int getHashInt() const;

private:
  std::string _toHex(const unsigned char *hash, size_t length);
  std::string _toHash(const std::string &src);
  std::string _hashedValue;
};

#endif /* SHAHASH_HPP */
