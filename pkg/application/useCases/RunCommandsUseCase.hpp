#ifndef RUNCOMMANDSUSECASE_HPP
#define RUNCOMMANDSUSECASE_HPP

#include "application/dto/RecievedMsgDTO.hpp"
#include "domain/message/BaseMessage.hpp"
#include "domain/message/IMessageAggregateRoot.hpp"

#include <stdexcept>
#include <string>

class RunCommandsUseCase {
public:
  static int execute(const RecievedMsgDTO &recieved);

private:
  RunCommandsUseCase();
  ~RunCommandsUseCase();
  RunCommandsUseCase(const RunCommandsUseCase &obj);
  RunCommandsUseCase &operator=(const RunCommandsUseCase &obj);
};

#endif /* RUNCOMMANDSUSECASE_HPP */
