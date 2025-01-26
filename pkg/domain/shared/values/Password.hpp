#ifndef PASSWORD_HPP
#define PASSWORD_HPP

#include "domain/shared/values/IPasswordAggregateRoot.hpp"
#include <iomanip>
#include <iostream>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <sstream>
#include <string>

class Password : virtual public IPasswordAggregateRoot {
public:
  Password(const std::string &password);
  Password(const Password &obj);
  ~Password();
  Password &operator=(const Password &obj);

  bool operator==(const Password &other) const;
  std::string getHash() const;

private:
  Password();
  std::string _toHex(const unsigned char *hash, size_t length);
  std::string _toHash(const std::string &password);
  std::string _hashedPassword;
};


#endif /* PASSWORD_HPP */
