
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

  //Desk
  sf::Texture desk_texture;
  sf::Sprite desk_sprite;

  //Font UI
  sf::Font ui_font;


  //All Menu texts
  sf::Text title_text;
  sf::Text start_text;
  sf::Text quit_text;

  //Score and lives text
  sf::Text score_text;
  sf::Text lives_text;
  sf::Text feedback_text;

  float feedback_timer = 0.0f;
  bool showing_feedback = false;

  void setupMenu();
  bool isMouseOverText(const sf::Text& text);
  bool isMouseOverSprite(const sf::Sprite& sprite);

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

  //stamp textures
  sf::Texture accept_texture;
  sf::Texture reject_texture;

  sf::Sprite accept_sprite;
  sf::Sprite reject_sprite;

  bool accept_hover = false;
  bool reject_hover = false;

  sf::Texture approved_stamp_texture;
  sf::Texture denied_stamp_texture;

  sf::Sprite approved_stamp_sprite;
  sf::Sprite denied_stamp_sprite;

  bool stamping = false;
  float stamp_timer = 0.0f;
  bool stamp_is_approved = false;
  float stamp_delay = 0.0f;

  //Scoring & lives
  int score = 0;
  int lives = 3;


  int current_animal_index;
  int current_passport_index;
  bool should_accept;
  bool passport_open = false;

  sf::Vector2f closed_passport_home_position;
  sf::Vector2f photo_offset = sf::Vector2f(-50.f, -20.f);

  float inspection_zone_x;

  bool loadTextures();

  void createSprites();

  void newAnimal();

  void openPassport();

  //For dragging 
  sf::Sprite* dragged_sprite;
  sf::Vector2f drag_offset;
  bool dragging = false;

  void startDragging(sf::Sprite* sprite, const sf::Vector2f& mouse_pos);

  void updateDragging();

  void stopDragging();

  bool isInInspectionZone(const sf::Sprite& sprite);

  //matching animals & passport
  bool passportMatchesAnimal() const;
  void nextAnimal();

};

#endif // PLATFORMER_GAME_H
