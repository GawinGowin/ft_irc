/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamasaw <syamasaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 15:07:35 by syamasaw          #+#    #+#             */
/*   Updated: 2025/01/05 15:19:37 by syamasaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(unsigned int port, std::string password) : port_(port), password_(password) {
  std::cout << "Server created" << std::endl;
}

Server::~Server() {
  std::cout << "Server destroyed" << std::endl;
}

Server::Server(const Server &src) {
  *this = src;
}

Server &Server::operator=(const Server &src) {
  if (this != &src) {
    port_ = src.port_;
    password_ = src.password_;
  }
  return *this;
}

u_short Server::getPortBigEndian() const {
  return htons(port_);
}

unsigned int Server::getPort() const {
  return port_;
}

std::string Server::getPassword() const {
  return password_;
}
