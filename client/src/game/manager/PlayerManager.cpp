#include "PlayerManager.hpp"
#include <SFML/Graphics.hpp>


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


// === Mutators ===
void PlayerManager::AddPlayer(const int id, const sf::Color& color) {
    _playersArray.push_back(Player(id, {0.0f, 0.0f}, color));
}

void PlayerManager::RemovePlayer(const int id) {
    // Remove the player from the array
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







// void PlayerManager::MovePlayer(const int id, const sf::Vector2f movement)
// {
//     bool succeed = false;
//     for(Player& player : _playersArray)
//     {
//         if(player.GetId() == id) {
//             player.MoveBy(movement);
//             succeed = true;
//         }
//     }
//     if(succeed == false) {
//         _playersArray.push_back(Player(id, {0.0f, 0.0f}, sf::Color::Blue));
//     }
// }
