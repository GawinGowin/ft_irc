#ifndef STARTSERVERUSECASE_HPP
#define STARTSERVERUSECASE_HPP

#include <stdexcept>

#include "application/dto/StartServerDTO.hpp"
#include "domain/shared/ISocketHandler.hpp"
#include "infra/service/ClientDBServiceLocator.hpp"
#include "infra/service/SocketHandlerServiceLocator.hpp"

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
