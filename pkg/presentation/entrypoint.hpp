#ifndef ENTRYPOINT_HPP
#define ENTRYPOINT_HPP

#include "application/factory/ServerFactory.hpp"
#include "application/useCases/MonitorSocketEventsUseCase.hpp"
#include "application/useCases/StartServerUseCase.hpp"

#include "domain/server/IServerRepository.hpp"
#include "domain/server/IServerService.hpp"

void entrypoint(int argc, char **argv);

#endif /* ENTRYPOINT_HPP */
