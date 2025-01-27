#include "domain/shared/values/Password.hpp"

Password::Password(const std::string &password) : _hashedPassword(_toHash(password)) {}

Password::Password(const Password &other) { _hashedPassword = other._hashedPassword; }

Password &Password::operator=(const Password &obj) {
  if (this != &obj) {
    this->_hashedPassword = obj._hashedPassword;
  }
  return *this;
}

Password::~Password() {}

bool Password::operator==(const Password &other) const {
  return _hashedPassword == other._hashedPassword;
};

std::string Password::getHash() const { return this->_hashedPassword; }

std::string Password::_toHex(const unsigned char *hash, size_t length) {
  std::ostringstream oss;
  for (size_t i = 0; i < length; ++i) {
    oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
  }
  return oss.str();
}

std::string Password::_toHash(const std::string &password) {
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
