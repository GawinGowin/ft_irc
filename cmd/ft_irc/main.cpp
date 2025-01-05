/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamasaw <syamasaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 15:12:59 by syamasaw          #+#    #+#             */
/*   Updated: 2025/01/05 15:21:49 by syamasaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../pkg/application/Server.hpp"

int main() {
  Server server(8080, "password");

  std::cout << "Port: " << server.getPort() << std::endl;
  std::cout << "Port (big endian): " << server.getPortBigEndian() << std::endl;
  std::cout << "Password: " << server.getPassword() << std::endl;

  return 0;
}
