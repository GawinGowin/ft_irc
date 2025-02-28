#ifndef MONITORSOCKETEVENTDTO_HPP
#define MONITORSOCKETEVENTDTO_HPP

class MonitorSocketEventDTO {
public:
  enum EventType { NewConnection, MessageReceived, Error };
  MonitorSocketEventDTO() : _event(NewConnection), _connectionFd(0) {};
  MonitorSocketEventDTO(const EventType type, const int connectionFd)
      : _event(type), _connectionFd(connectionFd) {};
  ~MonitorSocketEventDTO() {};
  MonitorSocketEventDTO(const MonitorSocketEventDTO &obj)
      : _event(obj._event), _connectionFd(obj._connectionFd) {};
  MonitorSocketEventDTO &operator=(const MonitorSocketEventDTO &obj) {
    this->_event = obj._event;
    this->_connectionFd = obj._connectionFd;
    return *this;
  };

  const EventType &getEvent() const { return _event; };
  const int &getConnectionFd() const { return _connectionFd; };

private:
  EventType _event;
  int _connectionFd;
};

#endif /* MONITORSOCKETEVENTDTO_HPP */
