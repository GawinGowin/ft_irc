#ifndef REMOVECONNECTIONUSECASE_HPP
#define REMOVECONNECTIONUSECASE_HPP

#include "application/serviceLocator/InmemoryClientDBServiceLocator.hpp"
#include "application/serviceLocator/LoggerServiceLocator.hpp"
#include "application/serviceLocator/SocketHandlerServiceLocator.hpp"
#include "domain/client/Client.hpp"
#include "infra/logger/MultiLogger.hpp"

#include <stdexcept>

class RemoveConnectionUseCase {
public:
  static void remove(int clientFd);

private:
  RemoveConnectionUseCase();
  ~RemoveConnectionUseCase();
  RemoveConnectionUseCase(const RemoveConnectionUseCase &obj);
  RemoveConnectionUseCase &operator=(const RemoveConnectionUseCase &obj);
};

#endif /* REMOVECONNECTIONUSECASE_HPP */
