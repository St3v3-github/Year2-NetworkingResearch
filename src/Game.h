
#ifndef PLATFORMER_GAME_H
#define PLATFORMER_GAME_H

#include "Client.h"
#include "Server.h"
#include <SFML/Graphics.hpp>

class Game
{
 public:
  Game(sf::RenderWindow& window, bool server);
  ~Game();
  bool init();
  void update(float dt);
  void render();
  void mouseClicked(sf::Event event);
  void keyPressed(sf::Event event);

 private:
  sf::RenderWindow& window;
  bool isServer = true;

  std::unique_ptr<Client> client = nullptr;
  std::unique_ptr<Server> server = nullptr;


};

#endif // PLATFORMER_GAME_H
