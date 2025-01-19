#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <poll.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#include <stdexcept>

const std::string SVR_ADDR = "127.0.0.1";
const int SVR_PORT = 8080;
const int BUF_SIZE = 1024;

static int communicate(int c_sock);

int main() {
  int w_addr;
  int c_sock;
  struct sockaddr_in a_addr;

  w_addr = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (w_addr == -1) {
    std::cerr << "socket() failed" << std::endl;
    return 1;
  }
  bzero(&a_addr, sizeof(struct sockaddr_in));
  // The sockaddr_in structure specifies the address family,
  // IP address, and port of the server to be connected to.
  a_addr.sin_family = AF_INET;
  a_addr.sin_port = htons((unsigned short)SVR_PORT);
  a_addr.sin_addr.s_addr = inet_addr(SVR_ADDR.c_str());

  if (bind(w_addr, (const struct sockaddr *)&a_addr, sizeof(a_addr)) == -1) {
    std::cerr << "Error: bind()" << std::endl;
    close(w_addr);
    return (-1);
  }
  if (listen(w_addr, SOMAXCONN) == -1) {
    std::cerr << "Error: listen()" << std::endl;
    close(w_addr);
    return (-1);
  }

  std::vector<pollfd> poll_fds;
  pollfd server_pollfd = {w_addr, POLLIN, 0};
  poll_fds.push_back(server_pollfd);
  std::cout << "Waiting for connections..." << std::endl;
  while (true) {
    int activity = poll(poll_fds.data(), poll_fds.size(), -1);
    if (activity < 0) {
      std::cerr << "Error: poll()" << std::endl;
      break;
    }
    for (size_t i = 0; i < poll_fds.size(); ++i) {
      if (poll_fds[i].revents & POLLIN) {
        if (poll_fds[i].fd == w_addr) {
          c_sock = accept(w_addr, NULL, NULL);
          if (c_sock == -1) {
            std::cerr << "Error: accept()" << std::endl;
            close(w_addr);
            return (-1);
          }
          std::cout << "New connection, socket fd is " << c_sock << "\n";
          pollfd client_pollfd = {c_sock, POLLIN, 0};
          poll_fds.push_back(client_pollfd);
        } else {
          int is_closed = 0;
          try {
            is_closed = communicate(poll_fds[i].fd); // 接続済のソケットでデータのやり取り
          } catch (const std::runtime_error &e) {
            std::cerr << "Error: "<< e.what() << std::endl;
            is_closed = 1;
          }
          if (is_closed) {
            close(poll_fds[i].fd);
            poll_fds.erase(poll_fds.begin() + i);
            --i;
          }
        }
      }
    }
  }
  close(w_addr);
  return (0);
}

static int communicate(int c_sock) {
  int recv_size;
  int send_size;
  char recv_buf[BUF_SIZE];
  char send_buf;

  recv_size = recv(c_sock, recv_buf, BUF_SIZE, 0); // 受信
  if (recv_size == -1) {
    throw std::runtime_error("recv() failed");
  } else if (recv_size == 0) {
    std::cout << "connection closed" << std::endl;
    return 1;
  }
  std::string recv_str = std::string(recv_buf, recv_size);

  if (recv_str == "exit") {
    send_buf = 0;
    send_size = send(c_sock, &send_buf, 1, 0);
    if (send_size == -1) {
      throw std::runtime_error("send() failed");
    }
    printf("A client exited\n");
    return 1;
  } else if (recv_str != "") {
    std::cout << "received: " << recv_str;
  } else {
    send_buf = 1;
    send_size = send(c_sock, &send_buf, 1, 0);
    if (send_size == -1) {
      throw std::runtime_error("send() failed");
    }
  }
  return 0;
}
