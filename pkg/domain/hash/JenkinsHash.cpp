#include "domain/hash/JenkinsHash.hpp"

static int jenkins_hash(char *k, int length, int initval);

JenkinsHash::JenkinsHash() : _hashedValue(0) {}

JenkinsHash::JenkinsHash(const std::string &rawString) : _hashedValue(_toHash(rawString)) {}

JenkinsHash::JenkinsHash(const JenkinsHash &obj) { *this = obj; }

JenkinsHash &JenkinsHash::operator=(const JenkinsHash &obj) {
  if (this != &obj) {
    this->_hashedValue = obj._hashedValue;
  }
  return *this;
}

JenkinsHash::~JenkinsHash() {}

bool JenkinsHash::operator==(const IHashAggregateRoot &other) const {
  return this->_hashedValue == other.getHashInt();
}

bool JenkinsHash::operator<(const IHashAggregateRoot &other) const {
  return this->_hashedValue < other.getHashInt();
}

std::string JenkinsHash::getHash() const {
  std::stringstream ss;
  ss << std::hex << this->_hashedValue;
  return ss.str();
}

long JenkinsHash::getHashLong() const { return static_cast<long>(this->_hashedValue); }

int JenkinsHash::getHashInt() const { return this->_hashedValue; }

int JenkinsHash::_toHash(const std::string &src) {
  char *str = const_cast<char *>(src.c_str());
  int length = static_cast<int>(src.length());
  int ret = jenkins_hash(str, length, 0);
  return ret;
}

/* The following code include lookup3.c, by Bob Jenkins, May 2006, Public Domain. */
/* (URL: <http://burtleburtle.net/bob/c/lookup3.c>) */

static inline int rot(int x, int k) { return (((x) << (k)) | ((x) >> (32 - (k)))); }

static inline void mix(int &a, int &b, int &c) {
  a -= c;
  a ^= rot(c, 4);
  c += b;
  b -= a;
  b ^= rot(a, 6);
  a += c;
  c -= b;
  c ^= rot(b, 8);
  b += a;
  a -= c;
  a ^= rot(c, 16);
  c += b;
  b -= a;
  b ^= rot(a, 19);
  a += c;
  c -= b;
  c ^= rot(b, 4);
  b += a;
}

static inline void final(int &a, int &b, int &c) {
  c ^= b;
  c -= rot(b, 14);
  a ^= c;
  a -= rot(c, 11);
  b ^= a;
  b -= rot(a, 25);
  c ^= b;
  c -= rot(b, 16);
  a ^= c;
  a -= rot(c, 4);
  b ^= a;
  b -= rot(a, 14);
  c ^= b;
  c -= rot(b, 24);
}

static int jenkins_hash(char *k, int length, int initval) {
  int a, b, c;

  a = b = c = 0xdeadbeef + length + initval;
  while (length > 12) {
    a += (k[0] + ((int)k[1] << 8) + ((int)k[2] << 16) + ((int)k[3] << 24));
    b += (k[4] + ((int)k[5] << 8) + ((int)k[6] << 16) + ((int)k[7] << 24));
    c += (k[8] + ((int)k[9] << 8) + ((int)k[10] << 16) + ((int)k[11] << 24));
    mix(a, b, c);
    length -= 12;
    k += 12;
  }
  switch (length) {
  case 12:
    c += ((int)k[11]) << 24; // fall through
  case 11:
    c += ((int)k[10] << 16); // fall through
  case 10:
    c += ((int)k[9] << 8); // fall through
  case 9:
    c += k[8]; // fall through
  case 8:
    b += ((int)k[7] << 24); // fall through
  case 7:
    b += ((int)k[6] << 16); // fall through
  case 6:
    b += ((int)k[5] << 8); // fall through
  case 5:
    b += k[4]; // fall through
  case 4:
    a += ((int)k[3] << 24); // fall through
  case 3:
    a += ((int)k[2] << 16); // fall through
  case 2:
    a += ((int)k[1] << 8); // fall through
  case 1:
    a += k[0];
    break;
  case 0:
    return c;
  }
  final(a, b, c);
  return c;
}
