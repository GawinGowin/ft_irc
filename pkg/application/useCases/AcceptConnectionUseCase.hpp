#ifndef ACCEPTCONNECTIONUSECASE_HPP
#define ACCEPTCONNECTIONUSECASE_HPP

#include "application/serviceLocator/ConfigsServiceLocator.hpp"
#include "application/serviceLocator/InmemoryClientDBServiceLocator.hpp"
#include "application/serviceLocator/SocketHandlerServiceLocator.hpp"
#include "domain/client/Client.hpp"
#include "domain/client/ConnectionInfo.hpp"
#include "infra/configs/ConfigsLoader.hpp"

#include <stdexcept>

// TODO: 接続情報分離により修正がある

class AcceptConnectionUseCase {
public:
  static void accept();

private:
  AcceptConnectionUseCase();
  ~AcceptConnectionUseCase();
  AcceptConnectionUseCase(const AcceptConnectionUseCase &obj);
  AcceptConnectionUseCase &operator=(const AcceptConnectionUseCase &obj);
};

#endif /* ACCEPTCONNECTIONUSECASE_HPP */
