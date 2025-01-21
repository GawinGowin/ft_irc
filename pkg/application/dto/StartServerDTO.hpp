#ifndef STARTSERVERDTO_HPP
#define STARTSERVERDTO_HPP

#include <string>

class StartServerDTO {
public:
  StartServerDTO(int argc, char **argv) : _argc(argc), _argv(argv){};
  ~StartServerDTO(){};
  StartServerDTO(const StartServerDTO &obj) : _argc(obj._argc), _argv(obj._argv){};
  StartServerDTO &operator=(const StartServerDTO &obj) {
    if (this != &obj) {
      this->_argc = obj._argc;
      this->_argv = obj._argv;
    }
    return *this;
  }

  const std::string getAddress() const {
    return "127.0.0.1"; // TODO: Parse from _argv
  }
  const int getPort() const {
    return 8080; // TODO: Parse from _argv
  }

private:
  int _argc;
  char **_argv;
};

#endif /* STARTSERVERDTO_HPP */
