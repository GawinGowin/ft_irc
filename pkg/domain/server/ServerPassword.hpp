#ifndef SERVERPASSWORD_HPP
#define SERVERPASSWORD_HPP

#include <iomanip>
#include <iostream>
#include <openssl/sha.h>
#include <sstream>
#include <string>

class ServerPassword {
public:
  ServerPassword(const std::string &password);
  ServerPassword(const ServerPassword &obj);
  ~ServerPassword();
  ServerPassword &operator=(const ServerPassword &obj);

  bool operator==(const ServerPassword &other) const;
  std::string getHash() const;

private:
  ServerPassword();
  std::string _toHex(const unsigned char *hash, size_t length);
  std::string _toHash(const std::string &password);
  std::string _hashedPassword;
};

#endif /* SERVERPASSWORD_HPP */
