//
// Created by Steven on 30/01/2023.
//

#include "Client.h"

void Client::connect()
{
  if(socket == nullptr)
  {
    socket = std::make_unique<sf::TcpSocket>();
  }

  if(socket->connect("127.0.0.1", 53000) == sf::Socket::Status::Done)
  {
    std::cout << "You're Connected!" << std::endl;
    connected = true;
    run();
  }

  else
  {
    std::cout << "Failed to Connect" << std::endl;
  }
}


void Client::input(sf::TcpSocket& iSocket) const
{
  while (running)
  {
    //grab user input
    std::string str;
    std::getline(std::cin, str);
    std::cin.clear();

    //is client still connected?
    if (connected)
    {
      //send the string and hope
      //also something to do with packet creation?
      iSocket.send(reinterpret_cast<char*>(str.data()), str.length());
    }
  }
}


void Client::run()
{
  running = true;
  std::thread input_thread ([&]{input(*socket); });
  input_thread.detach();

  //receive messages
  while (running && connected)
  {
    char static_buffer[1028];
    while (connected)
    {
      std::size_t received;
      auto status = socket->receive(static_buffer, 1028, received);

      if (status == sf::Socket::Status::Disconnected)
      {
        connected = false;
        std::cout << "clean disconnection" << std::endl;
        socket->disconnect();
        break;
      }

      else
      {
        if (received < 1028) static_buffer[received] = '\0';  //treat all data as string and append null terminator
        std::cout << static_buffer << '\n';
      }
    }
  }
}
