
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
		if (!accept_texture.loadFromFile("../Data/Critter_crossing/UI/accept_button.png"))
		{
			std::cout << "Couldnt load accept button\n";

		}
		if (!reject_texture.loadFromFile("../Data/Critter_crossing/UI/reject_button.png"))
		{
			std::cout << "Couldnt load accept button\n";

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
	open_passport_sprite->setScale(0.65f, 0.65f);
	

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

	//Accept button
	accept_sprite.setTexture(accept_texture);
	accept_sprite.setScale(0.4f, 0.4f);

	sf::FloatRect ab = accept_sprite.getLocalBounds();
	accept_sprite.setOrigin(ab.width / 2.f, ab.height / 2.f);
	
	accept_sprite.setPosition(window.getSize().x * 0.78f, window.getSize().y * 0.88f);

	//Reject button
	reject_sprite.setTexture(reject_texture);
	reject_sprite.setScale(0.4f, 0.4f);

	sf::FloatRect rb = reject_sprite.getLocalBounds();
	reject_sprite.setOrigin(rb.width / 2.f, rb.height / 2.f);

	reject_sprite.setPosition(window.getSize().x * 0.62f, window.getSize().y * 0.88f);


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
	passport_photo_sprite->setTexture(animal_textures[current_passport_index], true);

	sf::FloatRect photo_bounds = passport_photo_sprite->getLocalBounds();
	passport_photo_sprite->setOrigin(photo_bounds.width / 2.0f, photo_bounds.height / 2.0f);
    passport_photo_sprite->setScale(0.55f, 0.55f);

	passport_open = false;

	//Resets the closed passport for a new round
	closed_passport_sprite->setColor(sf::Color(255, 255, 255, 255));
	closed_passport_sprite->setPosition(closed_passport_home_position);

}

void Game::nextAnimal()
{
	passport_open = false;
	dragging = false;
	dragged_sprite = nullptr;

	closed_passport_sprite->setPosition(closed_passport_home_position);

	newAnimal();
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

		if (dragging && dragged_sprite)
		{
			sf::Vector2i mouse = sf::Mouse::getPosition(window);
			sf::Vector2f mouse_f(static_cast<float>(mouse.x), static_cast<float>(mouse.y));

			dragged_sprite->setPosition(mouse_f + drag_offset);

		}

		if (passport_open)
		{
			accept_hover = isMouseOverSprite(accept_sprite);
			reject_hover = isMouseOverSprite(reject_sprite);

			if (accept_hover)
			{
				accept_sprite.setColor(sf::Color(255, 255, 255, 255));

			}
			else
			{
				accept_sprite.setColor(sf::Color(200, 200, 200, 255));

			}
			if (reject_hover)
			{
				reject_sprite.setColor(sf::Color(255, 255, 255, 255));
			}
			else
			{
				reject_sprite.setColor(sf::Color(200, 200, 200, 255));

			}
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

		//Inspection zone outline
		sf::FloatRect openbounds = open_passport_sprite->getGlobalBounds();

		sf::RectangleShape zone_outline;
		zone_outline.setPosition(openbounds.left, openbounds.top);
		zone_outline.setSize(sf::Vector2f(openbounds.width, openbounds.height));

		zone_outline.setFillColor(sf::Color(0, 0, 0, 0));  
		zone_outline.setOutlineColor(sf::Color::Red);
		zone_outline.setOutlineThickness(3.f);

		window.draw(zone_outline);

		if (passport_open == true)
		{
			zone_outline.setOutlineColor(sf::Color::Green);
			window.draw(zone_outline);

			window.draw(accept_sprite);
			window.draw(reject_sprite);

		}

		if (animal_sprite)
			window.draw(*animal_sprite);

		if (!passport_open)
		{
			if (closed_passport_sprite)
				window.draw(*closed_passport_sprite);
		}
		else
		{
			if (open_passport_sprite)
			{
				window.draw(*open_passport_sprite);
				
			}
			if (passport_photo_sprite)
			{
				window.draw(*passport_photo_sprite);
			}
		}

		
        

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
	  if (event.mouseButton.button == sf::Mouse::Left)
	  {

		  sf::Vector2i mouse = sf::Mouse::getPosition(window);
		  sf::Vector2f mouse_f(static_cast<float>(mouse.x), static_cast<float>(mouse.y));

		  

		  if (!passport_open && isMouseOverSprite(*closed_passport_sprite))
		  {
			  dragging = true;
			  dragged_sprite = closed_passport_sprite;
			  drag_offset = dragged_sprite->getPosition() - mouse_f;
			  return;
		  }

		  if (passport_open && isMouseOverSprite(accept_sprite))
		  {

			  if (passportMatchesAnimal())
			  {
				  std::cout << "you accepted a real passport\n";
			  }
			  else
			  {
				  std::cout << " you accepted a fake passport\n";
				  
			  }

			  nextAnimal();
			  return;

		  }
		  if (passport_open && isMouseOverSprite(reject_sprite))
		  {
			  if (!passportMatchesAnimal())
			  {
				  std::cout << "You rejectd a fake passport\n";
			  }
			  else
			  {
				  std::cout << " you rejected a real passport\n";
				  

			  }

			  nextAnimal();
			  return;
		  }
	  }

	  
  }
}

void Game::mouseReleased(sf::Event event)
{
	if (event.mouseButton.button == sf::Mouse::Left)
	{
		if (dragging && dragged_sprite == closed_passport_sprite)
		{
			if (isInInspectionZone(*closed_passport_sprite))
			{
				std::cout << "passport dropped in zone\n";

				passport_open = true;
				closed_passport_sprite->setColor(sf::Color(255, 255, 255, 0));

				open_passport_sprite->setPosition(window.getSize().x * 0.7f, window.getSize().y * 0.45f);

				sf::FloatRect openbounds = open_passport_sprite->getGlobalBounds();

				sf::Vector2f photopos;
				photopos.x = openbounds.left + openbounds.width * 0.195f;
				photopos.y = openbounds.top + openbounds.height * 0.67f;

				passport_photo_sprite->setPosition(photopos);

			}
			else
			{
				closed_passport_sprite->setPosition(closed_passport_home_position);
				std::cout << "Dropped outside zone\n";
			}
		}


		dragging = false;
		dragged_sprite = nullptr;
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

bool Game::isInInspectionZone(const sf::Sprite& sprite)
{
	sf::FloatRect passportBounds = open_passport_sprite->getGlobalBounds();

	return passportBounds.intersects(sprite.getGlobalBounds());

}
bool Game::passportMatchesAnimal() const
{
	return current_animal_index == current_passport_index;
}

