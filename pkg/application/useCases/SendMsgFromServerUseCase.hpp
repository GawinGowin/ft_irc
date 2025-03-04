#ifndef SENDMSGFROMSERVERUSECASE_HPP
#define SENDMSGFROMSERVERUSECASE_HPP

#include "application/dto/SendMsgDTO.hpp"
#include "application/serviceLocator/LoggerServiceLocator.hpp"
#include "application/serviceLocator/SocketHandlerServiceLocator.hpp"
#include "domain/client/IClientAggregateRoot.hpp"
#include "domain/message/MessageService.hpp"
#include "domain/message/MessageStream.hpp"
#include "infra/logger/MultiLogger.hpp"

class SendMsgFromServerUseCase {
public:
  static void send(SendMsgDTO &message);

private:
  SendMsgFromServerUseCase();
  ~SendMsgFromServerUseCase();
  SendMsgFromServerUseCase(const SendMsgFromServerUseCase &obj);
  SendMsgFromServerUseCase &operator=(const SendMsgFromServerUseCase &obj);
};

#endif /* SENDMSGFROMSERVERUSECASE_HPP */
