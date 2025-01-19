#ifndef STARTSERVERUSECASE_HPP
#define STARTSERVERUSECASE_HPP

#include <stdexcept>

#include "IServerRepository.hpp"

class StartServerUseCase {
  public:
    StartServerUseCase(IServerRepository *serverRepository);
    ~StartServerUseCase();
    void execute(const std::string &addr, const int &port, const std::string &password);

  private:
    StartServerUseCase();
    StartServerUseCase(const StartServerUseCase &obj);
    StartServerUseCase &operator=(const StartServerUseCase &obj);

    IServerRepository *_serverRepository;
};

#endif /* STARTSERVERUSECASE_HPP */
