#pragma once
#include "player.hpp"
#include <string>


class PlayerManager
{
public:
    PlayerManager();

    // === Accessors ===
    sf::Vector2f GetPlayerPosition(const int id);
    std::vector<Player> GetPlayersArray() const;

    // === Manager ===
    void ManagerAllPlayerIdsReceived(const std::vector<std::string>& idsArray);

    // === Mutators ===
    void AddPlayer(const int id);
    void RemovePlayer(const int id);
    void ChangePlayerPosition(const int id, const sf::Vector2f& newPosition);

    //void MovePlayer(const int id, const sf::Vector2f movement);

    // === Rendering ===
    void Render(sf::RenderWindow& window);

private:
    std::vector<Player> _playersArray;
};