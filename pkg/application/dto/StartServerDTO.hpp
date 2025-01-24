#ifndef STARTSERVERDTO_HPP
#define STARTSERVERDTO_HPP

#include <string>

class StartServerDTO {
public:
  StartServerDTO(int argc, char **argv)
      : _argc(argc), _argv(argv), _address(std::string("127.0.0.1")), _port(8080),
        _password(std::string("password")){};
  ~StartServerDTO(){};
  StartServerDTO(const StartServerDTO &obj)
      : _argc(obj._argc), _argv(obj._argv), _address(obj._address), _port(obj._port),
        _password(obj._password){};

  StartServerDTO &operator=(const StartServerDTO &obj) {
    if (this != &obj) {
      this->_argc = obj._argc;
      this->_argv = obj._argv;
    }
    return *this;
  }

  const std::string &getAddress() const { return this->_address; }
  const int &getPort() const { return this->_port; }
  const std::string &getPassword() const { return this->_password; }

private:
  int _argc;
  char **_argv;

  const std::string _address;
  const int _port;
  const std::string _password;
};

#endif /* STARTSERVERDTO_HPP */
