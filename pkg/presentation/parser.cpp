#include "parser.hpp"

// 文字列先頭のスペースを削除
static void eraseSpace(std::string *str) {
  size_t cnt = 0;
  while (cnt < str->length() && (*str)[cnt] == ' ') {
    ++cnt;
  }
  if (cnt > 0)
    str->erase(0, cnt);
}

Parser::Parser(std::string message) {
  // 510文字以上は切り捨て+CRLF付加
  if (message.length() >= 510) {
    message = message.substr(0, 510) + "\r\n";
  }
  eraseSpace(&message);

  // \r\nをチェック
  if (message.length() < 2 || message.substr(message.length() - 2) != "\r\n") {
    throw std::runtime_error("error CRLF");
  }
  std::cout << message << std::endl;
  // prefixを取得
  if (message[0] == ':') {
    size_t pos = message.find(' ');
    // スペースが見つからない=prefixのみでcommandがないのでエラー
    if (pos == std::string::npos) {
      throw std::runtime_error("Invalid message format");
    }
    this->_prefix = message.substr(1, pos - 1);
    message.erase(0, pos + 1);
  } else {
    this->_prefix = "";
  }
  eraseSpace(&message);
  std::cout << message << std::endl;
  if (message.length() == 2) {
    throw std::runtime_error("Invalid message format2");
  }

  // commandを取得
  size_t pos = message.find_first_of(" \r\n");
  if (pos == std::string::npos) {
    throw std::runtime_error("Invalid message format3");
  }
  this->_command = message.substr(0, pos);
  message.erase(0, pos);
  if (message.length() == 2) {
    return;
  }
  eraseSpace(&message);
  std::cout << message << std::endl;
  while (message.length() > 2) {
    // パラメータを取得
    if (message[0] == ':') {
      this->_params.push_back(message.substr(1, message.length() - 3));
      break;
    }
    size_t pos = message.find(' ');
    if (pos == std::string::npos) {
      this->_params.push_back(message.substr(0, message.length() - 2));
      break;
    }
    this->_params.push_back(message.substr(0, pos));
    message.erase(0, pos + 1);
    eraseSpace(&message);
  }
}

Parser::~Parser() {}

Parser::Parser(const Parser &other) { *this = other; }

Parser &Parser::operator=(const Parser &other) {
  if (this != &other) {
    this->_prefix = other._prefix;
    this->_command = other._command;
    this->_params = other._params;
  }
  return *this;
}

// int main(void) {
//   Parser parser("   :irc.example.net     001 user aaa:Welcome to the Internet Relay Network user!user@host\r\n");
//   std::cout << parser.getPrefix() << std::endl;
//   std::cout << parser.getCommand() << std::endl;
//   std::cout << parser.getParams().size() << std::endl;

//   // Parser parser2(":hoge \r\n");
//   // std::cout << parser2.getPrefix() << std::endl;
//   return 0;
// }