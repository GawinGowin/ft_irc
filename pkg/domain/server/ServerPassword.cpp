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

std::string ServerPassword::getHash() const { return this->_hashedPassword; }

std::string ServerPassword::_toHex(const unsigned char *hash, size_t length) {
  std::ostringstream oss;
  for (size_t i = 0; i < length; ++i) {
    oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
  }
  return oss.str();
}

std::string ServerPassword::_toHash(const std::string &password) {
  unsigned char hash[EVP_MAX_MD_SIZE];
  unsigned int hashLength = 0;
  EVP_MD_CTX *context = EVP_MD_CTX_new();
  if (!context) {
    throw std::runtime_error("Failed to create EVP_MD_CTX");
  }
  try {
    if (EVP_DigestInit_ex(context, EVP_sha256(), NULL) != 1) {
      throw std::runtime_error("Failed to initialize SHA256 context");
    }
    if (EVP_DigestUpdate(context, password.c_str(), password.size()) != 1) {
      throw std::runtime_error("Failed to update SHA256 hash");
    }
    if (EVP_DigestFinal_ex(context, hash, &hashLength) != 1) {
      throw std::runtime_error("Failed to finalize SHA256 hash");
    }
    EVP_MD_CTX_free(context);
    return _toHex(hash, hashLength);
  } catch (...) {
    EVP_MD_CTX_free(context);
    throw std::runtime_error("Failed to hash password");
  }
}
