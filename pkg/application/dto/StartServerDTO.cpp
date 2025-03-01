#include "application/dto/StartServerDTO.hpp"

StartServerDTO::StartServerDTO(int argc, char **argv)
    : _port(-1), _password(""), _isError(true), _errorMessage("Unknown error") {
  if (argc != 3) {
    this->_errorMessage = "Invalid number of arguments";
    return;
  }
  for (int i = 0; argv[1][i]; i++) {
    if (!std::isdigit(argv[1][i]) || argv[1][0] == '0') {
      this->_errorMessage = "Invalid port";
      return;
    }
  }
  std::stringstream ss(argv[1]);
  unsigned int port;
  ss >> port;
  if (ss.fail() || port > 65535) {
    this->_errorMessage = "Invalid port";
    return;
  }
  std::string password(argv[2]);
  if (password.empty() || (password.find(' ') != std::string::npos)) {
    this->_errorMessage = "Invalid password";
    return;
  }
  this->_port = port;
  this->_password = password;
  this->_isError = false;
  this->_errorMessage = "";
}
