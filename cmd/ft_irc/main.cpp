/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamasaw <syamasaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 15:12:59 by syamasaw          #+#    #+#             */
/*   Updated: 2025/01/05 15:50:46 by syamasaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../pkg/application/Server.hpp"

Server *processArgs(int argc, char **argv);

int main(int argc, char **argv) {
  Server *server = processArgs(argc, argv);
  if (!server) {
    return 1;
  }

  std::cout << "Port: " << server->getPort() << std::endl;
  std::cout << "Port (big endian): " << server->getPortBigEndian() << std::endl;
  std::cout << "Password: " << server->getPassword() << std::endl;

  delete server;
  return 0;
}

Server *processArgs(int argc, char **argv) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
    return nullptr;
  }
  std::stringstream ss(argv[1]);
  unsigned int port;
  ss >> port;
  if (ss.fail() || port > 65535) {
    std::cerr << "Invalid port" << std::endl;
    return nullptr;
  }
  std::string password(argv[2]);
  if (password.empty() || (password.find(' ') != std::string::npos)) {
    std::cerr << "Invalid password" << std::endl;
    return nullptr;
  }
  return new Server(port, password);
}
