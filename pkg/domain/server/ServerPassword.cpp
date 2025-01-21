#include "domain/server/ServerPassword.hpp"

ServerPassword::ServerPassword(const std::string &password) : _hashedPassword(_toHash(password)) {}

ServerPassword::ServerPassword(const ServerPassword &other) {
  _hashedPassword = other._hashedPassword;
}

ServerPassword &ServerPassword::operator=(const ServerPassword &obj) {
  if (this != &obj) {
    this->_hashedPassword = obj._hashedPassword;
  }
  return *this;
}

ServerPassword::~ServerPassword() {}

bool ServerPassword::operator==(const ServerPassword &other) const {
  return _hashedPassword == other._hashedPassword;
};

std::string ServerPassword::getHash() const {
  return this->_hashedPassword;
}

std::string ServerPassword::_toHex(const unsigned char *hash, size_t length) {
  std::ostringstream oss;
  for (size_t i = 0; i < length; ++i) {
    oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
  }
  return oss.str();
}

std::string ServerPassword::_toHash(const std::string &password) {
  unsigned char hash[SHA256_DIGEST_LENGTH];
  SHA256_CTX sha256;
  SHA256_Init(&sha256);
  SHA256_Update(&sha256, password.c_str(), password.size());
  SHA256_Final(hash, &sha256);
  return _toHex(hash, SHA256_DIGEST_LENGTH);
}
