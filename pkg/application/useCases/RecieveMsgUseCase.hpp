#ifndef RECIEVEMSGUSECASE_HPP
#define RECIEVEMSGUSECASE_HPP

#include "application/dto/MonitorSocketEventDTO.hpp"
#include "application/dto/RecievedMsgDTO.hpp"
#include "application/serviceLocator/InmemoryClientDBServiceLocator.hpp"
#include "application/serviceLocator/LoggerServiceLocator.hpp"
#include "application/serviceLocator/SocketHandlerServiceLocator.hpp"
#include "domain/client/IClientAggregateRoot.hpp"

#include <string>
#include <sys/socket.h>
#include <vector>

class RecieveMsgUseCase {
public:
  static std::vector<RecievedMsgDTO> recieve(const MonitorSocketEventDTO &event);

private:
  RecieveMsgUseCase();
  ~RecieveMsgUseCase();
  RecieveMsgUseCase(const RecieveMsgUseCase &obj);
  RecieveMsgUseCase &operator=(const RecieveMsgUseCase &obj);
};

#endif /* RECIEVEMSGUSECASE_HPP */
