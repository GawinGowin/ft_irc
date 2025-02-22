#ifndef STARTSERVERUSECASE_HPP
#define STARTSERVERUSECASE_HPP

#include <stdexcept>

#include "application/dto/StartServerDTO.hpp"
#include "application/serviceLocator/ConfigsServiceLocator.hpp"
#include "application/serviceLocator/InmemoryClientDBServiceLocator.hpp"
#include "application/serviceLocator/SocketHandlerServiceLocator.hpp"
#include "domain/client/Client.hpp"
#include "domain/shared/ISocketHandler.hpp"
#include "domain/shared/values/IHashAggregateRoot.hpp"
#include "infra/configs/ConfigsLoader.hpp"

// TODO: 接続情報分離により修正がある

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
