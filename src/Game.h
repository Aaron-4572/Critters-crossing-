
#ifndef PLATFORMER_GAME_H
#define PLATFORMER_GAME_H

#include <SFML/Graphics.hpp>
#include <vector>

class Game
{
 public:
  Game(sf::RenderWindow& window);
  ~Game();
  bool init();
  void update(float dt);
  void render();
  void mouseClicked(sf::Event event);
  void mouseReleased(sf::Event event);
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




  //Animals
  sf::Sprite* animal_sprite;

  sf::Sprite* closed_passport_sprite;

  sf::Sprite* open_passport_sprite;

  sf::Sprite* passport_photo_sprite;

  //Texture array
  std::vector<sf::Texture> animal_textures;

  //Passport textures
  sf::Texture closed_passport_texture;
  sf::Texture open_passport_texture;

  int current_animal_index;
  int current_passport_index;
  bool should_accept;
  bool passport_open;

  sf::Vector2f closed_passport_home_position;

  float inspection_zone_x;

  bool loadTextures();

  void createSprites();

  void newAnimal();

  void openPassport();

  //For dragging 
  sf::Sprite* dragged_sprite;

  bool dragging;

  sf::Vector2f drag_offset;

  void startDragging(sf::Sprite* sprite, const sf::Vector2f& mouse_pos);

  void updateDragging();

  void stopDraggin();

  bool isInInspectionZone(const sf::Sprite& sprite);

};

#endif // PLATFORMER_GAME_H
