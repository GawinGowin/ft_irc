#ifndef ICLIENTAGGREGATEROOT_HPP
#define ICLIENTAGGREGATEROOT_HPP

#include <poll.h>

class IClientAggregateRoot {
  public: 
    virtual ~IClientAggregateRoot() {};

    virtual const int &getId() const = 0;
    virtual const int &getSocketFd() const = 0;
    virtual const pollfd &getPollfd() const = 0;
    virtual bool operator==(const IClientAggregateRoot &other) const = 0;
    virtual IClientAggregateRoot *clone() const = 0;
};

#endif /* ICLIENTAGGREGATEROOT_HPP */
