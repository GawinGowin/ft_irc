#ifndef ICHANNELREPOSITORY_HPP
#define ICHANNELREPOSITORY_HPP

#include "domain/channel/IChannelAggregateRoot.hpp"
#include "domain/shared/values/IHashAggregateRoot.hpp"

#include <vector>

/**
 * @class IChannelRepository
 * @brief チャンネルリポジトリのインターフェースを定義するクラス
 *
 * このインターフェースは、チャンネルの追加、一覧取得、取得、更新、削除の機能を提供します。
 */
class IChannelRepository {
public:
  virtual ~IChannelRepository() {};
  virtual void add(const IChannelAggregateRoot &channel) = 0;
  virtual IChannelAggregateRoot *get(const std::string &name) = 0;
  virtual void update(const std::string &name, const IChannelAggregateRoot &newData) = 0;
  virtual void remove(const std::string &name) = 0;
};

#endif /* ICHANNELREPOSITORY_HPP */
