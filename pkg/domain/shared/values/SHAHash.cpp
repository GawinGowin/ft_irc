#include "domain/shared/values/SHAHash.hpp"

SHAHash::SHAHash() : _hashedValue("A") {}

SHAHash::SHAHash(const std::string &rawPassword) : _hashedValue(_toHash(rawPassword)) {}

SHAHash::SHAHash(const SHAHash &other) { _hashedValue = other._hashedValue; }

SHAHash &SHAHash::operator=(const SHAHash &obj) {
  if (this != &obj) {
    this->_hashedValue = obj._hashedValue;
  }
  return *this;
}

SHAHash::~SHAHash() {}

bool SHAHash::operator==(const IHashAggregateRoot &other) const {
  return _hashedValue == other.getHash();
};

bool SHAHash::operator<(const IHashAggregateRoot &other) const {
  return _hashedValue < other.getHash();
}

std::string SHAHash::getHash() const { return this->_hashedValue; }

long SHAHash::getHashLong() const {
  // SHA256 hash is 32 bytes long (256 bits) so return the initial 64 bits as a long
  std::stringstream ss;
  ss << std::hex << this->_hashedValue.substr(0, 16);
  long value;
  ss >> value;
  return value;
}

int SHAHash::getHashInt() const {
  // SHA256 hash is 32 bytes long (256 bits) so return the initial 32 bits as an int
  std::stringstream ss;
  ss << std::hex << this->_hashedValue.substr(0, 8);
  int value;
  ss >> value;
  return value;
}

std::string SHAHash::_toHex(const unsigned char *hash, size_t length) {
  std::ostringstream oss;
  for (size_t i = 0; i < length; ++i) {
    oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
  }
  return oss.str();
}

std::string SHAHash::_toHash(const std::string &password) {
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
  } catch (std::runtime_error &e) {
    EVP_MD_CTX_free(context);
    throw e;
  } catch (...) {
    EVP_MD_CTX_free(context);
    throw std::runtime_error("Failed to hash password");
  }
}
