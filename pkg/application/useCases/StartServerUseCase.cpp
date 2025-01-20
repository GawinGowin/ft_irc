#include "StartServerUseCase.hpp"

StartServerUseCase::StartServerUseCase(
    IServerRepository *serverRepository, IServerService *serverServiceRepository)
    : _serverRepository(serverRepository), _serverServiceRepository(serverServiceRepository) {}

StartServerUseCase::~StartServerUseCase() {}

StartServerDTO
StartServerUseCase::execute(const std::string &addr, const int &port, const std::string &password) {
  std::vector<pollfd> fds;

  this->_serverRepository->setParams(addr, port);
  this->_serverRepository->setPassword(password);
  try {
    this->_serverRepository->establishConnection();
  } catch (std::runtime_error &e) {
    throw std::runtime_error(std::string("StartServerUseCase: ") + e.what());
  }
  fds.push_back({this->_serverRepository->getWatchingAddress(), POLLIN, 0});
  std::vector<int> clientFds = this->_serverRepository->getAcceptedClients();
  for (size_t i = 0; i < clientFds.size(); ++i) {
    pollfd client_pollfd = {clientFds[i], POLLIN, 0};
    fds.push_back(client_pollfd);
  }
  this->_serverRepository->registerClientById(this->_serverRepository->getWatchingAddress());
  return StartServerDTO(fds);
}
