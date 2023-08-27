#include "Player.hpp"
#include "config.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <cassert>


const float DEFAULT_SPEED = 300.0f;       /// Default speed of the player.
const float DEFAULT_FRAME_TIME = 0.016;   /// Default frame time, assuming 60 FPS.


/**
 * Constructs a new Player object.
 * Initializes the player attributes.
 */
Player::Player()
    : _position(600.0f, 300.0f),
      _speed(DEFAULT_SPEED),
      _size(30.0f, 30.0f),
      _shape(_size)
{
    _shape.setFillColor(sf::Color::Green);
    _shape.setPosition(_position);
}

/**
 * Updates the player's position based on keyboard inputs.
 */
void Player::Update()
{
    // Keyboard controls
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
        _position.y -= _speed * DEFAULT_FRAME_TIME;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        _position.y += _speed * DEFAULT_FRAME_TIME;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        _position.x -= _speed * DEFAULT_FRAME_TIME;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        _position.x += _speed * DEFAULT_FRAME_TIME;

    // Boundary checks
    //assert(_position.x >= 0.0f && _position.x <= WINDOW_WIDTH && "The player is outside the screen");
    //assert(_position.y >= 0.0f && _position.y <= WINDOW_HEIGHT && "The player is outside the screen");
    
    _shape.setPosition(_position);
}

/**
 * Render the player
 */
void Player::Render(sf::RenderWindow& window) const
{
    window.draw(_shape);
}