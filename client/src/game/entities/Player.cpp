#include "Player.hpp"
#include "config.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <cassert>


const float DEFAULT_SPEED = 300.0f;       /// Default speed of the player.
const sf::Vector2f DEFAULT_SIZE = {30.0f, 30.0f};



Player::Player(const int id, const sf::Vector2f& position, const sf::Color& color)
    : _id(id),
      _position(position),
      _speed(DEFAULT_SPEED),
      _size(DEFAULT_SIZE),
      _shape(_size)
{
    _shape.setFillColor(color);
    _shape.setPosition(_position);
}

// === Accessors ===
int Player::GetId() const {
    return _id;
}

float Player::GetSpeed() const {
    return _speed;
}

sf::Vector2f Player::GetPosition() const {
    return _position;
}


// === Mutators ===
void Player::SetPosition(const sf::Vector2f& newPosition) {
    _position = newPosition;
}


// === Rendering ===
void Player::Render(sf::RenderWindow& window)
{
    _shape.setPosition(_position);
    window.draw(_shape);
}