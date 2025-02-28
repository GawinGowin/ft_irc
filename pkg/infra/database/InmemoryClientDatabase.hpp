#ifndef INMEMORYCLIENTDATABASE_HPP
#define INMEMORYCLIENTDATABASE_HPP

#include "domain/client/IClientAggregateRoot.hpp"
#include "domain/client/IClientRepository.hpp"

#include <algorithm>
#include <poll.h>
#include <stdexcept>
#include <vector>

class InmemoryClientDatabase : virtual public IClientRepository {
public:
  InmemoryClientDatabase();
  ~InmemoryClientDatabase();
  InmemoryClientDatabase(const InmemoryClientDatabase &other);
  InmemoryClientDatabase &operator=(const InmemoryClientDatabase &other);

  void add(const IClientAggregateRoot &client);
  const std::vector<IClientAggregateRoot *> &list();
  const std::vector<pollfd> &listPollfds();
  IClientAggregateRoot *getById(const std::string &id);
  IClientAggregateRoot *getByFd(const int fd);
  void update(const std::string &id, const IClientAggregateRoot &newData);
  void remove(const std::string &id);
  void clear();
  void removeFdsByFd(const int fd);
  size_t size() const;

private:
  std::vector<IClientAggregateRoot *> _clients;
  std::vector<pollfd> _cachedPollfds;
  bool _isPollfdsCached;
};

#endif /* INMEMORYCLIENTDATABASE_HPP */
