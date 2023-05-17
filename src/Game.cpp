
#include "Game.h"
#include <iostream>

Game::Game(sf::RenderWindow& game_window, bool server)
  : window(game_window), isServer(server)
{
  srand(time(NULL));
}

Game::~Game()
{

}

bool Game::init()
{
  if (isServer)
  {
    server = std::make_unique<Server>();
    server->init();
    server->run();
  }

  else
  {
    client = std::make_unique<Client>();
    client -> connect();
  }

  return true;
}

void Game::update(float dt)
{

}

void Game::render()
{
  //rendering goes here
}

void Game::mouseClicked(sf::Event event)
{
  //get the click position
  sf::Vector2i click = sf::Mouse::getPosition(window);


}

void Game::keyPressed(sf::Event event)
{

}


