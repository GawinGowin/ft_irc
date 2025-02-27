#ifndef PASSWORD_HPP
#define PASSWORD_HPP

#include <string>

class Password {
public:
  Password() : _password(""), _ismutable(true) {}
  Password(const std::string &password) : _password(password), _ismutable(false) {}
  ~Password() {}
  Password(const Password &other) { *this = other; }
  Password &operator=(const Password &other) {
    if (this != &other) {
      this->_password = other._password;
      this->_ismutable = other._ismutable;
    }
    return *this;
  }

  bool operator==(const Password &other) const { return this->_password == other._password; }

  const std::string &getPassword() const { return this->_password; }

  int setPassword(const std::string &password) {
    if (this->_ismutable) {
      this->_password = password;
      this->_ismutable = false;
      return 0;
    }
    return 1;
  }

private:
  std::string _password;
  bool _ismutable;
};

#endif /* PASSWORD_HPP */
