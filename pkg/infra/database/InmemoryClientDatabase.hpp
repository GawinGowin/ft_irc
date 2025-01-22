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
  const std::vector<IClientAggregateRoot*> &list();
  const std::vector<pollfd> &listPollfds();
  const IClientAggregateRoot &getById(const int id);
  void update(const int id, const IClientAggregateRoot &newData);
  void remove(const int id);

private:
  std::vector<IClientAggregateRoot *> _clients;
  std::vector<pollfd> _cachedPollfds;
  bool _isPollfdsCached;
};

#endif /* INMEMORYCLIENTDATABASE_HPP */
