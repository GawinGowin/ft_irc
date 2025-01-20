#ifndef STARTSERVERDTO_HPP
#define STARTSERVERDTO_HPP

#include <poll.h>

class StartServerDTO {
public:
  StartServerDTO(std::vector<pollfd> fds) : _fds(fds){};
  ~StartServerDTO(){};
  StartServerDTO(const StartServerDTO &obj) : _fds(obj._fds){};
  StartServerDTO &operator=(const StartServerDTO &obj) {
    if (this != &obj) {
      this->_fds = obj._fds;
    }
    return *this;
  };
  
  const pollfd *getFds() const { return this->_fds.data(); };

private:
  std::vector<pollfd> _fds;
};

#endif /* STARTSERVERDTO_HPP */
