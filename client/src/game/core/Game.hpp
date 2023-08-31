#pragma once
#include "PlayerManager.hpp"
#include <SFML/Graphics.hpp>
#include "NetworkInterface.hpp"
#include <SFML/System.hpp>


class Game
{
public:
    Game(sf::RenderWindow& window);
    void Update();
    void Render();

private:
    void _HandleInputs();
    void _HandleEvents();
    void _HandlePackets(ENetPacket* packet);

    PlayerManager _playerManager;
    sf::RenderWindow& _window;
    NetworkInterface& _networkInterface;
    int _myId;

    sf::Time _lastUpdateTimeEvent1;
    sf::Clock _clock;

    bool _running;
};