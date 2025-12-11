
#include "Game.h"
#include <iostream>
#include <ctime>
#include <cstdlib>

Game::Game(sf::RenderWindow& game_window)
  : current_state(GameState::MENU)
  , window(game_window)
  

{
  std::srand(static_cast<unsigned int>(std::time(nullptr)));

  animal_sprite = nullptr;
  closed_passport_sprite = nullptr;
  open_passport_sprite = nullptr;
  passport_photo_sprite = nullptr;
  dragged_sprite = nullptr;

  dragging = false;
  passport_open = false;

  current_animal_index = 0;
  current_passport_index = 0;

  should_accept = false;

  inspection_zone_x = window.getSize().x * 0.55f;

  closed_passport_home_position = sf::Vector2f(window.getSize().x * 0.25f, window.getSize().y - desk_sprite.getGlobalBounds().height - 70.f);


}

Game::~Game()
{
	delete animal_sprite;
	delete closed_passport_sprite;
	delete open_passport_sprite;
	delete passport_photo_sprite;

	animal_sprite = nullptr;
	closed_passport_sprite = nullptr;
	open_passport_sprite = nullptr;
	passport_photo_sprite = nullptr;


}

bool Game::init()
{
	//Setting background textures
	if (!background_texture.loadFromFile("../Data/Critter_crossing/background.jpg"))
	{
		std::cout << "Could not load background image";
    }
	else
	{
		background_sprite.setTexture(background_texture);
	}
	

	//setting UI textures
	if (!ui_font.loadFromFile("../Data/Fonts/OpenSans-Bold.ttf"))
	{
		std::cout << "could not load font";
		return false;
	}

	setupMenu();

	if (!loadTextures())
	{
		std::cout << "Textures failed to load";
	}

	createSprites();

	newAnimal();


	return true;
}

void Game::setupMenu()
{
	//Title text
	title_text.setFont(ui_font);
	title_text.setString("Critters Crossing");
	title_text.setCharacterSize(64);
	title_text.setFillColor(sf::Color::White);

	//setting titel location
	sf::FloatRect title_bounds = title_text.getLocalBounds();
	title_text.setOrigin(title_bounds.width / 2.0f, title_bounds.height / 2.0f);
	title_text.setPosition(window.getSize().x / 2.0f, 120.0f);

	//Start game text
	start_text.setFont(ui_font);
	start_text.setString("Start Game");
	start_text.setCharacterSize(36);
	start_text.setFillColor(sf::Color::Yellow);

	//setting start text loaction
	sf::FloatRect start_bounds = start_text.getLocalBounds();
	start_text.setOrigin(start_bounds.width / 2.0f, start_bounds.height / 2.0f);
	start_text.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);

	//Quit game text
	quit_text.setFont(ui_font);
	quit_text.setString("Quit");
	quit_text.setCharacterSize(36);
	quit_text.setFillColor(sf::Color::Yellow);

	//setting quit text location
	sf::FloatRect quit_bounds = quit_text.getLocalBounds();
	quit_text.setOrigin(quit_bounds.width / 2.0f, quit_bounds.height / 2.0f);
	quit_text.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f + 60.0f);


}

bool Game::loadTextures()
{
	bool all_ok = true;

	animal_textures.resize(9);

	auto loadAnimal = [&](int index, const std::string& path)
		{
			if (!animal_textures[index].loadFromFile(path))
			{
				std::cout << "couldnt load animal textures" <<path << "\n";
				all_ok = false;
			}
		};


	//loading animal textures
	loadAnimal(0, "../Data/Critter_crossing/Animals/walrus.png");
	loadAnimal(1, "../Data/Critter_crossing/Animals/buffalo.png");
	loadAnimal(2, "../Data/Critter_crossing/Animals/elephant.png");
	loadAnimal(3, "../Data/Critter_crossing/Animals/giraffe.png");
	loadAnimal(4, "../Data/Critter_crossing/Animals/gorilla.png");
	loadAnimal(5, "../Data/Critter_crossing/Animals/moose.png");
	loadAnimal(6, "../Data/Critter_crossing/Animals/narwhal.png");
	loadAnimal(7, "../Data/Critter_crossing/Animals/penguin.png");
	loadAnimal(8, "../Data/Critter_crossing/Animals/chicken.png");

	    //pasport
		if (!closed_passport_texture.loadFromFile("../Data/Critter_crossing/passports/closed_passport.png"))
		{
			std::cout << "couldnt load closed passport texture\n";
			all_ok = false;
	    }

		if (!open_passport_texture.loadFromFile("../Data/Critter_crossing/passports/open_passport.png"))
		{
			std::cout << "couldnt load open passport texture\n";
			all_ok = false;

		}
		if (!desk_texture.loadFromFile("../Data/Critter_crossing/UI/Desk.png"))
		{
			std::cout << "Desk texture couldnt load\n";
			all_ok = false;
		}

		return all_ok;
}

void Game::createSprites()
{
	animal_sprite = new sf::Sprite();
    closed_passport_sprite = new sf::Sprite();
	open_passport_sprite = new sf::Sprite();
	passport_photo_sprite = new sf::Sprite();

	//closed passport texture
	closed_passport_sprite->setTexture(closed_passport_texture, true);

	//closed passport positions 
	sf::FloatRect closed_bounds = closed_passport_sprite->getLocalBounds();
	closed_passport_sprite->setOrigin(closed_bounds.width / 2.0f, closed_bounds.height / 2.0f);

	closed_passport_sprite->setPosition(closed_passport_home_position);
	closed_passport_sprite->setScale(0.3f, 0.3f);

	//open passport texture
	open_passport_sprite->setTexture(open_passport_texture, true);

	//open passport position & inspection zone
	sf::FloatRect open_bounds = open_passport_sprite->getLocalBounds();
    open_passport_sprite->setOrigin(open_bounds.width / 2.0f, open_bounds.height / 2.0f);
	open_passport_sprite->setPosition(window.getSize().x * 0.70f, window.getSize().y * 0.45f);

	sf::FloatRect dummy_animal_bounds = animal_sprite->getLocalBounds();
	animal_sprite->setOrigin(dummy_animal_bounds.width / 2.0f, dummy_animal_bounds.height / 2.0f);

	animal_sprite->setPosition(window.getSize().x * 0.20f, window.getSize().y * 0.60f);

	sf::FloatRect dummy_photo_bounds = closed_passport_sprite->getLocalBounds();
	passport_photo_sprite->setOrigin(dummy_photo_bounds.width / 2.0f, dummy_photo_bounds.height / 2.0f);

	//Desk setup
	desk_sprite.setTexture(desk_texture);

	float scale_x = window.getSize().x / desk_sprite.getLocalBounds().width;
	float scale_y = 0.5f;

	desk_sprite.setScale(scale_x, scale_y);

	desk_sprite.setPosition(0, window.getSize().y - desk_sprite.getGlobalBounds().height);

}


void Game::newAnimal()
{
	if (animal_textures.empty())
	{
		std::cout << "Warning: animal_textures is empty, cannot generate new animal.\n";
		return;
	}

	//generating randoms for animal and passport photo
	current_animal_index = std::rand() % animal_textures.size();
	current_passport_index = std::rand() % animal_textures.size();

	should_accept = (current_animal_index == current_passport_index);

	//setting said produces
    animal_sprite->setTexture(animal_textures[current_animal_index], true);

	//Re-centering
	sf::FloatRect animal_bounds = animal_sprite->getLocalBounds();
	animal_sprite->setOrigin(animal_bounds.width / 2.0f,
		animal_bounds.height / 2.0f);

	//Positioning
	animal_sprite->setPosition(
		window.getSize().x * 0.20f,
		window.getSize().y * 0.60f);

	//passports turn
	sf::FloatRect photo_bounds = passport_photo_sprite->getLocalBounds();
	passport_photo_sprite->setOrigin(photo_bounds.width / 2.0f, photo_bounds.height / 2.0f);

	passport_photo_sprite->setScale(0.6f, 0.6f);

	passport_open = false;

	closed_passport_sprite->setPosition(closed_passport_home_position);

}


void Game::update(float dt)
{
	(void)dt;

	if (current_state == GameState::MENU)
	{
		if (isMouseOverText(start_text))
		{
			start_text.setFillColor(sf::Color::White);
		}
		else
		{
			start_text.setFillColor(sf::Color::Yellow);
		}

		if (isMouseOverText(quit_text))
		{
			quit_text.setFillColor(sf::Color::White);
		}
		else
		{
			quit_text.setFillColor(sf::Color::Yellow);
		}

	}
	else if (current_state == GameState::PLAYING)
	{
		//Brightens and dims closed passport when hovering
		if (isMouseOverSprite(*closed_passport_sprite))
		{
			closed_passport_sprite->setColor(sf::Color(255, 255, 255, 255));
		}
		else
		{
			closed_passport_sprite->setColor(sf::Color(200, 200, 200, 200));
		}
	}
	else if (current_state == GameState::EXIT)
	{

	}
}

void Game::render()
{
	window.draw(background_sprite);

	if (current_state == GameState::MENU)
	{
		window.draw(title_text);
		window.draw(start_text);
		window.draw(quit_text);
	}
	else if(current_state == GameState::PLAYING)
	{
		window.draw(desk_sprite);

		if (animal_sprite)
			window.draw(*animal_sprite);

		if (closed_passport_sprite)
			window.draw(*closed_passport_sprite);
        

	}
}

void Game::mouseClicked(sf::Event event)
{
  //get the click position
  sf::Vector2i click_position = sf::Mouse::getPosition(window);

  if (current_state == GameState::MENU)
  {
	  if (event.mouseButton.button == sf::Mouse::Left)
	  {
		  sf::Vector2f click_position_f(static_cast<float>(click_position.x), static_cast<float>(click_position.y));

		  if (start_text.getGlobalBounds().contains(click_position_f))
		  {
			  current_state = GameState::PLAYING;
		  }
		  else if (quit_text.getGlobalBounds().contains(click_position_f))
		  {
			  current_state = GameState::EXIT;
		  }
	  }
  }
  else if (current_state == GameState::PLAYING)
  {

  }
}

void Game::keyPressed(sf::Event event)
{
	if (current_state == GameState::PLAYING)
	{
		if (event.key.code == sf::Keyboard::Escape)
		{
			current_state = GameState::MENU;
		}
	}
	else if (current_state == GameState::MENU)
	{
		if (event.key.code == sf::Keyboard::Enter)
		{
			current_state = GameState::PLAYING;
		}
		else if (event.key.code == sf::Keyboard::Enter)
		{
			current_state = GameState::EXIT;
		}
	}
}

bool Game::isMouseOverText(const sf::Text& text)
{
	sf::Vector2i mouse_position = sf::Mouse::getPosition(window);

	sf::Vector2f mouse_position_f(static_cast<float>(mouse_position.x), static_cast<float>(mouse_position.y));

	return text.getGlobalBounds().contains(mouse_position_f);
}

bool Game::isMouseOverSprite(const sf::Sprite& sprite)
{
	sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
	sf::Vector2f mouse_pos_f(static_cast<float>(mouse_pos.x), static_cast<float>(mouse_pos.y));

	return sprite.getGlobalBounds().contains(mouse_pos_f);
}

