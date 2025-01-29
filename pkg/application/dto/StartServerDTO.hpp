#ifndef STARTSERVERDTO_HPP
#define STARTSERVERDTO_HPP

#include <sstream>
#include <stdexcept>
#include <string>

class StartServerDTO {
public:
  StartServerDTO(int argc, char **argv);
  ~StartServerDTO() {};
  StartServerDTO(const StartServerDTO &obj)
      : _address(obj._address), _port(obj._port), _password(obj._password) {};

  StartServerDTO &operator=(const StartServerDTO &obj) {
    (void)obj;
    return *this;
  }

  const std::string &getAddress() const { return this->_address; }
  const int &getPort() const { return this->_port; }
  const std::string &getPassword() const { return this->_password; }

private:
  std::string _address;
  int _port;
  std::string _password;
};

#endif /* STARTSERVERDTO_HPP */
