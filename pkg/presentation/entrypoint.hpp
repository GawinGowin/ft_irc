#ifndef ENTRYPOINT_HPP
#define ENTRYPOINT_HPP

#include "application/dto/MonitorSocketEventDTO.hpp"
#include "application/dto/StartServerDTO.hpp"
#include "application/useCases/AcceptConnectionUseCase.hpp"
#include "application/useCases/MonitorSocketEventsUseCase.hpp"
#include "application/useCases/RecieveMsgUseCase.hpp"
#include "application/useCases/RemoveConnectionUseCase.hpp"
#include "application/useCases/RunCommandsUseCase.hpp"
#include "application/useCases/StartServerUseCase.hpp"
#include "signal.hpp"

extern volatile sig_atomic_t g_signal;

void entrypoint(int argc, char **argv);

#endif /* ENTRYPOINT_HPP */
