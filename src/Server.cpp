// Server.cpp
#include <memory>

#include "Server.h"
#include <iostream> // Don't forget to include this for std::cout

void Server::init()
{
  if (listener == nullptr) listener = std::make_unique<sf::TcpListener>();

  // Port to listener bind
  if (listener->listen(53000) != sf::Socket::Done)
  {

  }
}

void Server::listen(sf::TcpSocket& cSocket)
{
  bool continue_receiving = true;
  std::vector<char> buffer(1028);
  char data[1028];
  std::size_t received;
  auto message = reinterpret_cast<char*>(buffer.data());


  std::lock_guard<std::mutex> lck(mtx);

  for (auto& connection : connections)
  {
    connection->send(message, buffer.size());
  }

  while (continue_receiving)
  {

    auto status = cSocket.receive(data, 1028, received);
    if (status == sf::Socket::Status::Disconnected)
    {
      continue_receiving = false; // client disconnected :(
      std::cout << "Disconnected " << std::endl;
    }

    // Add a null terminator (He won't be back) and print the received data as a string
    if (received < 1028)
    {
      data[received] = '\0';
    }

    // Assuming 'send' function is defined in the Server class, send the data back to the client
    send(data);

    std::cout << reinterpret_cast<char*>(data) << '\n';

    // This loop seems redundant; you may want to remove it
    while (continue_receiving)
    {

    }
  }
}
#include "Server.h"
#include <cstring>


void Server::send(const char* data) {
  std::lock_guard<std::mutex> lck(mtx);


  for (const auto& connection : connections) {
    connection->send(data, std::strlen(data) + 1);
  }
}


void Server::run() {
  while (running) {

    auto cSock = std::make_unique<sf::TcpSocket>();

    if (listener->accept(*cSock) == sf::Socket::Done) {
      std::cout << "Client Accepted @ " << cSock->getRemotePort() << std::endl;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(100));


    std::cout << "detected disconnection\n";

    {

      std::lock_guard<std::mutex> lck(mtx);


      for (int i = 0; i < connections.size(); ++i) {
        if (connections[i]->getLocalPort() == cSock->getLocalPort()) {
          continue;
        }
        connections.erase(std::next(connections.begin(), i));
        break;
      }
    }

    // Remove the last element in connections if it's not empty
    if (!connections.empty()) {
      connections.pop_back();
    }
  }
}
