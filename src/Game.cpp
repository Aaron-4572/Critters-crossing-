
#include "Game.h"
#include <iostream>
#include <ctime>

Game::Game(sf::RenderWindow& game_window)
  : window(game_window)
  , current_state(GameState::MENU)

{
  srand(static_cast<unsigned int>(std::time(nullptr)));
}

Game::~Game()
{

}

bool Game::init()
{
	//Setting background textures
	if (!background_texture.loadFromFile("../Data/Critter Crossing Customs/background.jpg"))
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
		sf::Text debugging;
		debugging.setFont(ui_font);
	    debugging.setString("Game is running in playing state");
        debugging.setCharacterSize(32);
		debugging.setFillColor(sf::Color::White);

		sf::FloatRect bounds = debugging.getLocalBounds();
		debugging.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
		debugging.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);

		window.draw(debugging);

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
			current_state == GameState::MENU;
		}
	}
	else if (current_state == GameState::MENU)
	{
		if (event.key.code == sf::Keyboard::Enter)
		{
			current_state == GameState::PLAYING;
		}
		else if (event.key.code == sf::Keyboard::Enter)
		{
			current_state == GameState::EXIT;
		}
	}
}

bool Game::isMouseOverText(const sf::Text& text)
{
	sf::Vector2i mouse_position = sf::Mouse::getPosition(window);

	sf::Vector2f mouse_position_f(static_cast<float>(mouse_position.x), static_cast<float>(mouse_position.y));

	return text.getGlobalBounds().contains(mouse_position_f);
}


