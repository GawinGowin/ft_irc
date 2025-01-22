#ifndef STARTSERVERUSECASE_HPP
#define STARTSERVERUSECASE_HPP

#include <stdexcept>

#include "application/dto/StartServerDTO.hpp"
#include "application/serviceLocator/InmemoryClientDBServiceLocator.hpp"
#include "application/serviceLocator/SocketHandlerServiceLocator.hpp"
#include "domain/client/Client.hpp"
#include "domain/server/ServerPassword.hpp"
#include "domain/shared/ISocketHandler.hpp"

class StartServerUseCase {
public:
  StartServerUseCase(const StartServerDTO &dto);
  ~StartServerUseCase();
  void execute();

private:
  StartServerUseCase();
  StartServerUseCase(const StartServerUseCase &obj);
  StartServerUseCase &operator=(const StartServerUseCase &obj);
};

#endif /* STARTSERVERUSECASE_HPP */
