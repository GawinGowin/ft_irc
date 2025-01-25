#ifndef RECIEVEMSGUSECASE_HPP
#define RECIEVEMSGUSECASE_HPP

#include "application/dto/MonitorSocketEventDTO.hpp"
#include "application/serviceLocator/SocketHandlerServiceLocator.hpp"
#include <string>
#include <sys/socket.h>

class RecieveMsgUseCase {
public:
  static std::string recieve(const MonitorSocketEventDTO &event);

private:
  RecieveMsgUseCase();
  ~RecieveMsgUseCase();
  RecieveMsgUseCase(const RecieveMsgUseCase &obj);
  RecieveMsgUseCase &operator=(const RecieveMsgUseCase &obj);
};

#endif /* RECIEVEMSGUSECASE_HPP */
