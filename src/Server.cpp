//
// Created by Steven on 30/01/2023.
//

#include "Server.h"


void Server::init()
{
  if (listener == nullptr) listener = std::make_unique<sf::TcpListener>();
  if(listener->listen(53000) != sf::Socket::Done)
  {
    std::cout << "Listener isn't bound to port" << std::endl;
  }
}


void Server::run()
{
  while (running)
  {
    // add pointer for new_connection and get reference to it
    sf::TcpSocket& cSock =
      connections.emplace_back(std::make_unique<sf::TcpSocket>()).operator*();

    // when we get a new_connection (socket)
    //  if new_connection invalid - return
    if (listener->accept(cSock) != sf::Socket::Done)
    {
      // don't add this client and perform any incorrect actions
      connections.pop_back();
      return;
    }

    else
    {
      std::cout << "Client Connected @ " << cSock.getRemotePort() << std::endl;
      workers.emplace_back([&]{
          listen(cSock); // listens for messages on the connected socket
          std::cout << "Client Disconnected \n"<< std::endl; // we know the socket disconnects, so remove from list
          std::lock_guard<std::mutex> lck(mutex); // guard access to socket list

          for (int i = 0; i < connections.size(); ++i)
          {
            if (connections[i]->getLocalPort() == cSock.getLocalPort())
              continue; // check if connection matches, else move onto next one

            connections.erase(std::next(connections.begin(), i)); // erase  the i-th element of the connections (the correct one)

            break; // we've changed the vector, so need to break out of loop
          }});
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

void Server::listen(sf::TcpSocket& cSocket)
{
  bool continue_receiving = true;

  char data[1028];
  std::size_t received;

  while (continue_receiving)
  {
    // is there data ready for us?
    auto status = cSocket.receive(data, 1028, received);

    if (status == sf::Socket::Status::Disconnected)
    {
      continue_receiving = false; // client disconnected
      std::cout << "Disconnected" << std::endl;
    }

    if (received < 1028) { data[received] = '\0';} // add a null terminator, print as string
    send(data);
    std::cout << reinterpret_cast<char*>(data) << '\n';
  }
  cSocket.disconnect();
}


void Server::send(std::string buffer)
{
  auto message = reinterpret_cast<char*>(buffer.data());

  std::lock_guard<std::mutex>lck(mutex);

  for(auto & connection : connections)
  {
    connection->send(message, buffer.size());
  }
}
