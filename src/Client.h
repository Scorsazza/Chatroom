//
// Created by Scors on 19/02/2023.
//

#ifndef CHATROOM_CLIENT_H
#define CHATROOM_CLIENT_H
#include <SFML/Network.hpp>
#include <iostream>
#include <atomic>
#include <thread>

class Client
{
 public:
  void connect();
  void input(sf::TcpSocket& iSocket) const;
  void run();

  std::atomic<bool> running   = false;
  std::atomic<bool> connected = false;

 private:
  std::unique_ptr<sf::TcpSocket> socket;


#endif // CHATROOM_CLIENT_H
};