#ifndef SOCKETHANDLERSERVICELOCATOR_HPP
#define SOCKETHANDLERSERVICELOCATOR_HPP

#include "domain/shared/values/IHashAggregateRoot.hpp"
#include "infra/socket/SocketHandler.hpp"
#include <stdexcept>

class SocketHandlerServiceLocator {
public:
  static void
  init(std::string address, const int port, const int maxConnections, const int maxBufferSize) {
    if (_service != NULL) {
      throw std::runtime_error("already initialized");
    }
    _service = new SocketHandler(address, port, maxConnections, maxBufferSize);
  }

  static SocketHandler &get() {
    if (_service == NULL) {
      throw std::runtime_error("not initialized");
    }
    return *_service;
  }

  static void cleanup() {
    if (_service != NULL) {
      delete _service;
      _service = NULL;
    }
  }

private:
  static SocketHandler *_service;
};

#endif /* SOCKETHANDLERSERVICELOCATOR_HPP */
