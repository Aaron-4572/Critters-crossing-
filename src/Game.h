
#ifndef PLATFORMER_GAME_H
#define PLATFORMER_GAME_H

#include <SFML/Graphics.hpp>

class Game
{
 public:
  Game(sf::RenderWindow& window);
  ~Game();
  bool init();
  void update(float dt);
  void render();
  void mouseClicked(sf::Event event);
  void keyPressed(sf::Event event);

 private:

  enum class GameState
  {
	MENU,
	PLAYING,
	EXIT
  };

  GameState current_state;

  sf::RenderWindow& window;

  //background
  sf::Texture background_texture;
  sf::Sprite background_sprite;

  //Font UI
  sf::Font ui_font;

  //All title texts
  sf::Text title_text;
  sf::Text start_text;
  sf::Text quit_text;

  void setupMenu();
  bool isMouseOverText(const sf::Text& text);



  

};

#endif // PLATFORMER_GAME_H
