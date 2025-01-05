/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syamasaw <syamasaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 14:58:23 by syamasaw          #+#    #+#             */
/*   Updated: 2025/01/05 15:10:46 by syamasaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
# define SERVER_H

#include <string>
#include <iostream>
#include <netinet/in.h>

class Server {
  private:
    unsigned int port_;
    std::string password_;
  public:
    Server(unsigned int port = 8080, std::string password = "password");
    ~Server();
    Server(const Server &src);
    Server &operator=(const Server &src);

    u_short getPort() const;
    std::string getPassword() const;
};

#endif
