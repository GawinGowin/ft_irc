#ifndef ENTRYPOINT_HPP
#define ENTRYPOINT_HPP

#include "application/useCases/StartServerUseCase.hpp"
#include "application/useCases/MonitorSocketEventsUseCase.hpp"
#include "application/dto/StartServerDTO.hpp"

void entrypoint(int argc, char **argv);

#endif /* ENTRYPOINT_HPP */
