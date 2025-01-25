#include "StartServerDTO.hpp"

StartServerDTO::StartServerDTO(int argc, char **argv) {
  if (argc != 3) {
    throw std::runtime_error("Invalid number of arguments");
  }
  for (int i = 0; argv[1][i]; i++) {
    if (!std::isdigit(argv[1][i]) || argv[1][0] == '0') {
      throw std::runtime_error("Invalid port");
    }
  }
  std::stringstream ss(argv[1]);
  unsigned int port;
  ss >> port;
  if (ss.fail() || port > 65535) {
    throw std::runtime_error("Invalid port");
  }
  std::string password(argv[2]);
  if (password.empty() || (password.find(' ') != std::string::npos)) {
    throw std::runtime_error("Invalid password");
  }
  this->_port = port;
  this->_password = password;
  this->_address = std::string("127.0.0.1");
}
