#include "PlayerManager.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <array>

/// variables
std::array<sf::Color, 4> colorsArray{sf::Color::Blue, sf::Color::Red, sf::Color::Green, sf::Color::Magenta};



PlayerManager::PlayerManager()
    : _playersArray()
{}


// === Accessors ===
std::vector<Player> PlayerManager::GetPlayersArray() const {
    return _playersArray;
}

sf::Vector2f PlayerManager::GetPlayerPosition(const int id) 
{
    for(Player& player : _playersArray)
    {
        if(player.GetId() == id) {
            return player.GetPosition();
        }
    }
}


// === Managers ===
void PlayerManager::ManagerAllPlayerIdsReceived(const std::vector<std::string>& idsArray)
{
    // Ajoute tout les joueurs déja connecté dans le tableau
    for(const std::string& idStr : idsArray) {
        if(idStr != "-1") {
            int id{stoi(idStr)};
            AddPlayer(id);
            std::cout << "Le joueur " << id << " etait deja connecté" << std::endl;
        }
    }
}




// === Mutators ===
void PlayerManager::AddPlayer(const int id) {
    _playersArray.push_back(Player(id, {0.0f, 0.0f}, colorsArray[id]));
}

void PlayerManager::RemovePlayer(const int id) {
    int index;

    for(unsigned int i{0} ; i < _playersArray.size() ; ++i) {
        if(_playersArray[i].GetId() == id)
            index = i;
    }
    _playersArray.erase(_playersArray.begin() + index);
}

void PlayerManager::ChangePlayerPosition(const int id, const sf::Vector2f& newPosition) 
{
    for(Player& player : _playersArray)
    {
        if(player.GetId() == id) {
            player.SetPosition(newPosition);
        }
    }
}



// === Rendering ===
void PlayerManager::Render(sf::RenderWindow& window)
{
    for(Player& player : _playersArray)
        player.Render(window);
}