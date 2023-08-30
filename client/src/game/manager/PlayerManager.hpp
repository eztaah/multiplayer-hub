#pragma once
#include "player.hpp"


class PlayerManager
{
public:
    PlayerManager();

    // === Accessors ===
    sf::Vector2f GetPlayerPosition(const int id);

    std::vector<Player> GetPlayersArray() const;


    // === Mutators ===
    void AddPlayer(const int id, const sf::Color& color);
    void RemovePlayer(const int id);
    void ChangePlayerPosition(const int id, const sf::Vector2f& newPosition);
    
    //void MovePlayer(const int id, const sf::Vector2f movement);

    // === Rendering ===
    void Render(sf::RenderWindow& window);

private:
    std::vector<Player> _playersArray;
};