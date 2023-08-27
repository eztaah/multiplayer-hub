#pragma once 
#include <SFML/Graphics.hpp>


class Player
{
public:
    Player(); // Constructor
    void Update(); // Update player's position based on input
    void Render(sf::RenderWindow& window) const; // Render player on the window

private:
    sf::Vector2f _position;
    float _speed;
    sf::Vector2f _size;
    sf::RectangleShape _shape;
};