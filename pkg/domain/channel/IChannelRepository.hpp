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
  virtual ~IChannelRepository(){};
  /**
 * @brief チャンネルを追加する
 * @param channel 追加するチャンネルのアグリゲートルート
 */
  virtual void add(const IChannelAggregateRoot &channel) = 0;
  /**
 * @brief すべてのチャンネルを一覧取得する
 * @return チャンネルのアグリゲートルートのポインタのベクター
 */
  virtual const std::vector<IChannelAggregateRoot *> list() = 0;
  /**
 * @brief 指定されたIDと名前のチャンネルを取得する
 * @param id チャンネルのID
 * @param name チャンネルの名前
 * @return 指定されたIDと名前のチャンネルのアグリゲートルートのポインタ
 * 
 * id(ハッシュ化した値)とNameで検索を行うのはngircdの内部実装に従ったものです。
 */
  virtual const IChannelAggregateRoot *
  get(const IHashAggregateRoot &id, const std::string &name) = 0;
  /**
 * @brief 指定されたIDと名前のチャンネルを更新する
 * @param id チャンネルのID
 * @param name チャンネルの名前
 * @param newData 更新する新しいデータ
 * 
 * この関数は例外をスローしないため、事前にgetメソッドを用いてチャンネルの存在確認を行う必要があります。
 */
  virtual void update(
      const IHashAggregateRoot &id,
      const std::string &name,
      const IChannelAggregateRoot &newData) = 0;
  /**
   * @brief 指定されたIDと名前のチャンネルを削除する
   * @param id チャンネルのID
   * @param name チャンネルの名前
   * 
   * この関数は例外をスローしないため、事前にgetメソッドを用いてチャンネルの存在確認を行う必要があります。
   */
  virtual void remove(const IHashAggregateRoot &id, const std::string &name) = 0;
};

#endif /* ICHANNELREPOSITORY_HPP */
