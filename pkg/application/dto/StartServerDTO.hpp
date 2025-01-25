#ifndef STARTSERVERDTO_HPP
#define STARTSERVERDTO_HPP

#include <string>

class StartServerDTO {
public:
  StartServerDTO(std::pair<int, std::string> portPassword)
      : _address(std::string("127.0.0.1")), _port(portPassword.first), _password(portPassword.second) {};
  ~StartServerDTO(){};
  StartServerDTO(const StartServerDTO &obj)
      : _address(obj._address), _port(obj._port), _password(obj._password){};

  StartServerDTO &operator=(const StartServerDTO &obj) {
    (void)obj;
    return *this;
  }

  const std::string &getAddress() const { return this->_address; }
  const int &getPort() const { return this->_port; }
  const std::string &getPassword() const { return this->_password; }

private:
  const std::string _address;
  const int _port;
  const std::string _password;
};

#endif /* STARTSERVERDTO_HPP */
