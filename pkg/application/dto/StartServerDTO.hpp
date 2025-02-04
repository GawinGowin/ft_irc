#ifndef STARTSERVERDTO_HPP
#define STARTSERVERDTO_HPP

#include <sstream>
#include <stdexcept>
#include <string>

class StartServerDTO {
public:
  StartServerDTO(int argc, char **argv);
  ~StartServerDTO(){};
  StartServerDTO(const StartServerDTO &obj) : _port(obj._port), _password(obj._password){};

  StartServerDTO &operator=(const StartServerDTO &obj) {
    if (this != &obj) {
      this->_port = obj._port;
      this->_password = obj._password;
    }
    return *this;
  }

  const int &getPort() const { return this->_port; }
  const std::string &getPassword() const { return this->_password; }

private:
  int _port;
  std::string _password;
};

#endif /* STARTSERVERDTO_HPP */
