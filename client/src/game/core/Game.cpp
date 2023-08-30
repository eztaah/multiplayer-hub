#include "Game.hpp"

#include <iostream>
#include <enet/enet.h>
#include "config.hpp"
#include "NetworkInterface.hpp"
#include <string>
#include <sstream>
#include <array>


//////////////////// UTILS ///////////////////////
std::vector<std::string> splitString(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

enum receiveFromServer
{
    RECEIVE_MY_ID = 1,  // 1 | id
    RECEIVE_IDs_ALL_CONNECTED_PLAYERS = 2,  // 2 | id | id | id | ...
    RECEIVE_NEW_PLAYER_CONNECTED = 3,   // 3 | id

    RECEIVE_NEW_POSITION_OTHER_PLAYER = 4   // 4 | id | posX | posY
};



bool EventTriggered(sf::Time interval, sf::Time& lastUpdateTime, sf::Clock& clock)
{
    sf::Time currentTime = clock.getElapsedTime();
    if (currentTime - lastUpdateTime >= interval)
    {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

std::array<sf::Color, 4> colorsArray{sf::Color::Blue, sf::Color::Red, sf::Color::Green, sf::Color::Magenta};




//////////////////// PUBLIC /////////////////////
Game::Game(sf::RenderWindow& window)
    : _playerManager(),
      _window(window),
      _networkInterface(NetworkInterface::GetInstance()),    // Récupère l'instance de networkInterface
      _myId(-1),
      _lastUpdateTimeEvent1(),
      _clock()
{
    // === Initialize ENet and connect to the server ===  
    _networkInterface.Initialize();
    _networkInterface.ConnectToServer("localhost", 7000);
}

void Game::Update() {
    if(_myId != -1)
        _HandleInputs();

    _HandlePackets();
}


void Game::Render() {

    // Clear the window with white color
    _window.clear(sf::Color::White);

    // Display all the player
    if(_myId != -1)
        _playerManager.Render(_window);

    // End the current frame and display its contents on screen
    _window.display();
}



//////////////////// PRIVATE /////////////////////
void Game::_HandleInputs()
{   
    if(WINDOW_IS_FOCUSED)
    {
        // Create position
        sf::Vector2f newPosition{_playerManager.GetPlayerPosition(_myId)};

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
            newPosition.y -= 300.0f * DEFAULT_FRAME_TIME;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            newPosition.y += 300.0f * DEFAULT_FRAME_TIME;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
            newPosition.x -= 300.0f * DEFAULT_FRAME_TIME;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            newPosition.x += 300.0f * DEFAULT_FRAME_TIME;

        if(_myId !=-1)
        {
            // Update player newPosition
            _playerManager.ChangePlayerPosition(_myId, newPosition);

            // Send the newPosition to the server
            if(EventTriggered(sf::seconds(0.5f), _lastUpdateTimeEvent1, _clock))
            _networkInterface.SendNewPositionToServer(_myId, newPosition);
        }
    }
}

void Game::_HandlePackets()
{
    // Met à jour le tableau de packets
    _networkInterface.UpdateReceivedEvents();

    // récupère le tableau avec les packets
    std::vector<std::string> packetsArray{_networkInterface.GetAllReceivedPackets()};

    for(std::string& content : packetsArray)
    {
        // Split the data
        std::vector<std::string> splittedData{splitString(content, '|')};
    
        // Get the event type
        std::string eventTypeStr{splittedData[0]};
        int eventType{stoi(eventTypeStr)};

        // Gestion des différents cas
        switch (eventType)
        {
            case RECEIVE_MY_ID:
            {
                // Get my id 
                _myId = stoi(splittedData[1]);
                std::cout << "L'ID qui m'est attribué est le : " << _myId << std::endl;

                // Add my player to the array of player
                _playerManager.AddPlayer(_myId, colorsArray[_myId]);
                break;
            }

            case RECEIVE_IDs_ALL_CONNECTED_PLAYERS:
            {
                // Ajoute tout les joueurs déja connecté dans le tableau
                for(std::string& idStr : splittedData) {
                    int id{stoi(idStr)};
                    if(id != _myId)
                        _playerManager.AddPlayer(id, colorsArray[id]);
                }
                break;
            }

            case RECEIVE_NEW_PLAYER_CONNECTED:
            {
                // Get the player id
                int playerId = stoi(splittedData[1]);

                // Add it to the player array
                _playerManager.AddPlayer(playerId, colorsArray[playerId]);
                printf("Player %i vient de se connecter.", playerId);
                break;
            }

            case RECEIVE_NEW_POSITION_OTHER_PLAYER:
            {
                // Get the player id 
                int playerId = stoi(splittedData[1]);

                // Get the new player position
                sf::Vector2f newPosition;
                newPosition.x = stoi(splittedData[2]);
                newPosition.y = stoi(splittedData[3]);

                // Met à jour la nouvelle position du joueur dans le tableau
                _playerManager.ChangePlayerPosition(playerId, newPosition);
                printf("Le player %i est maintenant à la position {%f, %f}\n", playerId, newPosition.x, newPosition.y);
                break;
            }

            default:
                printf("Evement inconnu...");
                exit(EXIT_FAILURE);
                break;
        }
    }
}