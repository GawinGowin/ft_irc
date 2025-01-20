#ifndef STARTSERVERUSECASE_HPP
#define STARTSERVERUSECASE_HPP

#include <stdexcept>

#include "IServerRepository.hpp"
#include "IServerService.hpp"
#include "StartServerDTO.hpp"

class StartServerUseCase {
public:
  StartServerUseCase(IServerRepository *serverRepository, IServerService *serverServiceRepository);
  ~StartServerUseCase();
  StartServerDTO execute(const std::string &addr, const int &port, const std::string &password);

private:
  StartServerUseCase();
  StartServerUseCase(const StartServerUseCase &obj);
  StartServerUseCase &operator=(const StartServerUseCase &obj);

  IServerRepository *_serverRepository;
  IServerService *_serverServiceRepository;
};

#endif /* STARTSERVERUSECASE_HPP */
