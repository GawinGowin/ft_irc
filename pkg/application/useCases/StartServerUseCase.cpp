#include "StartServerUseCase.hpp"

StartServerUseCase::StartServerUseCase(IServerRepository *serverRepository)
    : _serverRepository(serverRepository) {}

StartServerUseCase::~StartServerUseCase() {}

void StartServerUseCase::execute(const std::string &addr, const int &port, const std::string &password) {
  this->_serverRepository->setParams(addr, port);
  this->_serverRepository->setPassword(password);
  try {
    this->_serverRepository->establishConnection();
  } catch (std::runtime_error &e) {
    throw std::runtime_error(std::string("StartServerUseCase: ") + e.what());
  }
}
