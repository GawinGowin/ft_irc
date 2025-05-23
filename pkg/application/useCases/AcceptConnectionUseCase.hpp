#ifndef ACCEPTCONNECTIONUSECASE_HPP
#define ACCEPTCONNECTIONUSECASE_HPP

#include "application/serviceLocator/ConfigsServiceLocator.hpp"
#include "application/serviceLocator/InmemoryClientDBServiceLocator.hpp"
#include "application/serviceLocator/LoggerServiceLocator.hpp"
#include "application/serviceLocator/SocketHandlerServiceLocator.hpp"
#include "domain/client/Client.hpp"
#include "domain/client/ConnectionInfo.hpp"
#include "infra/configs/ConfigsLoader.hpp"
#include "infra/logger/MultiLogger.hpp"

#include <stdexcept>

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
