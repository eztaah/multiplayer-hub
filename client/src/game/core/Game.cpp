#include "Game.hpp"


Game::Game(sf::RenderWindow& window)
    : _player(),
      _window(window)
{}

void Game::Update() {
    _player.Update();
}

void Game::Render() {

    // Clear the window with white color
    _window.clear(sf::Color::White);

    // Render the player
    _player.Render(_window);

    // End the current frame and display its contents on screen
    _window.display();
}