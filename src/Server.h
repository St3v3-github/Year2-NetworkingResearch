//
// Created by Steven on 30/01/2023.
//

#pragma once
#include <sfml/Network.hpp>
#include <thread>
#include <list>
#include <iostream>
#include <mutex>
#include <memory>

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

  std::mutex mutex;

  bool running = true;

};

