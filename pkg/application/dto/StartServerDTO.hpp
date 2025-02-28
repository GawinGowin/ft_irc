#ifndef STARTSERVERDTO_HPP
#define STARTSERVERDTO_HPP

#include <sstream>
#include <stdexcept>
#include <string>

class StartServerDTO {
public:
  StartServerDTO(int argc, char **argv);
  ~StartServerDTO() {};
  StartServerDTO(const StartServerDTO &obj) { *this = obj; };
  StartServerDTO &operator=(const StartServerDTO &obj) {
    if (this != &obj) {
      this->_port = obj._port;
      this->_password = obj._password;
      this->_isError = obj._isError;
      this->_errorMessage = obj._errorMessage;
    }
    return *this;
  }

  const int &getPort() const { return this->_port; }
  const std::string &getPassword() const { return this->_password; }

  bool isError() const { return this->_isError; }
  const std::string &getErrorMessage() const { return this->_errorMessage; }

private:
  int _port;
  std::string _password;
  bool _isError;
  std::string _errorMessage;

  StartServerDTO();
};

#endif /* STARTSERVERDTO_HPP */
