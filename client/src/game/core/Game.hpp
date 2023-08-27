#pragma once
#include "Player.hpp"
#include <SFML/Graphics.hpp>


class Game
{
public:
    Game(sf::RenderWindow& window);
    void Update();
    void Render();

private:
    Player _player;
    sf::RenderWindow& _window;
};