#include "infra/socket/SocketHandler.hpp"
#include "gtest/gtest.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <poll.h>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

TEST(SocketHandlerTest, InitializationSuccess) {
  int port = 9090;
  std::string address = "127.0.0.1";
  SocketHandler handler(address, port, 10, 1024);
  EXPECT_EQ(handler.getPort(), port);
  EXPECT_EQ(handler.getMaxConnections(), 10);
  EXPECT_EQ(handler.getMaxBufferSize(), 1024);
  EXPECT_FALSE(handler.isListening());
  // コンストラクトしただけではリッスンしていない
  // リッスンとバインド開始
  handler.initializeSocket();
  EXPECT_NE(handler.getServerSocket(), -1);
  EXPECT_TRUE(handler.isListening());
  // 二重でバインドは出来ないはず
  EXPECT_THROW({ handler.initializeSocket(); }, std::runtime_error);
  // bind関数を用いてバインドする
  struct sockaddr_in addr {};
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr(address.c_str());
  ; // 任意のアドレス
  addr.sin_port = htons(port);
  int s = socket(AF_INET, SOCK_STREAM, 0);
  EXPECT_EQ(bind(s, (struct sockaddr *)&addr, sizeof(addr)), -1);
}

TEST(SocketHandlerTest, InitializationFailure) {
  EXPECT_THROW(
      {
        SocketHandler handler("invalid_address", 8080, 10, 1024);
        handler.initializeSocket();
      },
      std::runtime_error);
  EXPECT_THROW(
      {
        SocketHandler handler("2001:0db8:1234:5678:90ab:cdef:0000:0000", 8080, 10, 1024);
        handler.initializeSocket();
      },
      std::runtime_error);
  EXPECT_THROW(
      {
        SocketHandler handler("127.0.0.1", -1, 10, 1024);
        handler.initializeSocket();
      },
      std::runtime_error);
  EXPECT_THROW(
      {
        SocketHandler handler("127.0.0.1", -1, 10, 1024);
        handler.initializeSocket();
      },
      std::runtime_error);
  EXPECT_THROW(
      {
        SocketHandler handler("127.0.0.1", 70000, 10, 1024);
        handler.initializeSocket();
      },
      std::runtime_error);
  EXPECT_THROW(
      {
        SocketHandler handler("127.0.0.1", 8080, -1, 1024);
        handler.initializeSocket();
      },
      std::runtime_error);
  EXPECT_THROW(
      {
        SocketHandler handler("127.0.0.1", 8080, SOMAXCONN + 1, 1024);
        handler.initializeSocket();
      },
      std::runtime_error);
  EXPECT_THROW(
      {
        SocketHandler handler("127.0.0.1", 8080, 10, -1);
        handler.initializeSocket();
      },
      std::runtime_error);
  EXPECT_THROW(
      {
        SocketHandler handler("127.0.0.1", 8080, 10, 70000);
        handler.initializeSocket();
      },
      std::runtime_error);
}

TEST(SocketHandlerTest, AcceptConnectionSuccess) {
  SocketHandler server("127.0.0.1", 8081, 10, 1024);
  server.initializeSocket();

  // Start a client in a separate thread or process
  std::thread client_thread([]() {
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8081);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
    close(client_socket);
  });

  sockaddr_in client_addr;
  int client_socket = server.acceptConnection(&client_addr);
  EXPECT_NE(client_socket, -1);
  EXPECT_EQ(server.getMaxConnections(), 10);  // Check max connection not changed
  EXPECT_EQ(server.getMaxBufferSize(), 1024); // Check buffer size not changed

  client_thread.join();
  close(client_socket);
}

TEST(SocketHandlerTest, SendAndReceiveMessage) {
  SocketHandler server("127.0.0.1", 8083, 10, 1024);
  server.initializeSocket();

  std::thread client_thread([&server]() {
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8083);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));

    std::string message = "Hello from client";
    server.sendMsg(message, client_socket);
    std::string received_message = server.receiveMsg(client_socket);
    EXPECT_EQ(received_message, "Hello from server");
    close(client_socket);
  });

  sockaddr_in client_addr;
  int client_socket = server.acceptConnection(&client_addr);

  std::string received_message = server.receiveMsg(client_socket);
  EXPECT_EQ(received_message, "Hello from client");

  std::string message = "Hello from server";
  server.sendMsg(message, client_socket);

  client_thread.join();
  close(client_socket);
}
