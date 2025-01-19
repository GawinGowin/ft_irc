#ifndef ICLIENTREPOSITORY_HPP
#define ICLIENTREPOSITORY_HPP

class IClientRepository {
  public:
    virtual ~IClientRepository() = 0;
    virtual const int &getId() const = 0;
};

#endif /* ICLIENTREPOSITORY_HPP */
