//
// Created by Scors on 19/02/2023.
//

#include "Server.h"
#include <mutex>
void Server::init()
{
  if (listener == nullptr) listener = std::make_unique<sf::TcpListener>();
  //port to listener bind
  if (listener->listen(53000)  != sf::Socket::Done)
  {
    //error.
  }
}

void Server::listen(sf::TcpSocket& cSocket)
{
  bool continue_receiving = true;
  std::vector<char> buffer(1028);
  char data[1028];
  std::size_t received;
  auto message = reinterpret_cast<char*>(buffer.data());
  std::lock_guard<std::mutex>lck(std::mutex);

  for (auto & connection : connections)
  {
    connection->send(message, buffer.size());
  }
  while (continue_receiving)
  {
    //Is this the data we are looking for?
    auto status = cSocket.receive(data, 1028, received);
    if(status == sf::Socket::Status::Disconnected)
    {
      continue_receiving = false; // client disconnected :(
      std::cout << "Disconnected " << std::endl;
    }
    //add a null terminator (He won't be back) and print as string
    if (received < 1028) { data[received] = '\0';}
    send(data);

    std::cout << reinterpret_cast<char*>(data) << '\n';

  }
  cSocket.disconnect();
}



void Server::run()
{
  while (running)
  {
    sf::TcpSocket& cSock =
      connections.emplace_back(std::make_unique<sf::TcpSocket>()).operator*();
    if (listener->accept(cSock) != sf::Socket::Done)
      std::cout << "Client Accepted @ " << cSock.getRemotePort() << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    //Disconnected, please leave
    std::cout << "detected disconnection\n";
    std::lock_guard<std::mutex>lck(std::mutex);

    for(int i =0; i < connections.size(); ++i)
    {
      if (connections[i]->getLocalPort() == cSock.getLocalPort()) continue;

      connections.erase(std::next(connections.begin(), i));

      break;
    }
    {
      connections.pop_back();
      return;
    }
  }
}