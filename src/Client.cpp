//
// Created by Scors on 19/02/2023.
//

#include "Client.h"
void Client::input(sf::TcpSocket& iSocket)const
{
  while (running)
  {
    //User input grabby-thing
    std::string str;
    std::getline(std::cin, str);
    std::cin.clear();
    //Check if client is alive and connected
    if (connected)
    {
      //Send the string with crossed fingers and a pinch of hope for good measure
      iSocket.send(reinterpret_cast<char*>(str.data()), str.length());
    }
  }
}
void Client::connect()
{
  if (socket->connect("127.0.0.1", 53000) == sf::Socket::Status::Done)
  {
    std::cout << "You're Connected!" << std::endl;
    connected = true;
  }
  else
  {
    std::cout << "failed to connect";
  }


  running = true;
  std::thread input_thread([&] -> void { input(*socket); });
  input_thread.detach();

  //Message receiver
  while(running && connected)
  {
    char static_buffer[1028];
    while(connected)
    {
      std::size_t received;
      auto status = socket->receive(static_buffer, 1028, received);

      if(status == sf::Socket::Status::Disconnected)
      {
        connected = false;
        std::cout << "clean disconnection" << std::endl;
        socket->disconnect();
        break;
      }
      else
      {
        if (received < 1028) static_buffer[received] = '\0';
        std::cout << static_buffer << '\n';
      }
    }
  }
}


