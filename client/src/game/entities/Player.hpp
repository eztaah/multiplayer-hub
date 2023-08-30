#pragma once 
#include <SFML/Graphics.hpp>


class Player
{
public:
    Player(const int id, const sf::Vector2f& position, const sf::Color& color); // Constructor

    // === Accessors ===
    int GetId() const;
    float GetSpeed() const;
    sf::Vector2f GetPosition() const;

    // === Mutators ===
    void SetPosition(const sf::Vector2f& newPosition);
    void MoveBy(const sf::Vector2f& movement);

    // === Rendering ===
    void Render(sf::RenderWindow& window); // Render player on the window

private:
    int _id;
    sf::Vector2f _position;
    float _speed;
    sf::Vector2f _size;
    sf::RectangleShape _shape;
};