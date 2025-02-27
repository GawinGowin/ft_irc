#ifndef RUNCOMMANDSUSECASE_HPP
#define RUNCOMMANDSUSECASE_HPP

#include "application/commands/Nick.hpp"
#include "application/commands/Pass.hpp"

#include "application/dto/RecievedMsgDTO.hpp"
#include "application/dto/SendMsgDTO.hpp"
#include "application/serviceLocator/LoggerServiceLocator.hpp"
#include "domain/message/IMessageAggregateRoot.hpp"
#include "domain/message/Message.hpp"
#include "infra/logger/MultiLogger.hpp"

#include <stdexcept>
#include <string>

class RunCommandsUseCase {
public:
  static SendMsgDTO execute(RecievedMsgDTO &recieved);

private:
  RunCommandsUseCase();
  ~RunCommandsUseCase();
  RunCommandsUseCase(const RunCommandsUseCase &obj);
  RunCommandsUseCase &operator=(const RunCommandsUseCase &obj);
};

#endif /* RUNCOMMANDSUSECASE_HPP */
