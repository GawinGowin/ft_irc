#ifndef MESSAGESTREAMVECTOR_HPP
#define MESSAGESTREAMVECTOR_HPP

#include "domain/message/MessageStream.hpp"
#include <cassert>
#include <vector>

class MessageStreamVector : public std::vector<MessageStream> {
public:
  // 各メッセージの送信結果を含むvectorを返す
  // 0は成功、0以外は失敗（エラーコード）
  std::vector<int> send() {
    std::vector<int> results;
    for (std::vector<MessageStream>::iterator it = this->begin(); it != this->end(); ++it) {
      int status = it->send();
      results.push_back(status);
    }
    assert(results.size() == this->size());
    return results;
  }
};

#endif /* MESSAGESTREAMVECTOR_HPP */
