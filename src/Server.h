//
// Created by Scors on 19/02/2023.
//
#include <SFML/Network.hpp>
#include <thread>
#include <list>
#include <iostream>
#include <mutex>
#include <memory>

#ifndef CHATROOM_SERVER_H
#define CHATROOM_SERVER_H
class Server
{
 public:
  void init();
  void run();
  void listen(sf::TcpSocket& cSocket);
  void send(std::string buffer);
 private:
  std::vector<std::thread> workers;
  std::vector<std::unique_ptr<sf::TcpSocket>> connections;

  std::unique_ptr<sf::TcpListener> listener;
  std::unique_ptr<sf::TcpSocket> socket;

  bool running = true;


};
#endif // CHATROOM_SERVER_H
