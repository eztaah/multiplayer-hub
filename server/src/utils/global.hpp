#pragma once
#include <enet/enet.h>
#include <SFML/System.hpp>


struct Client
{
    ENetPeer *peer;
    int id;
    sf::Vector2f position;
};