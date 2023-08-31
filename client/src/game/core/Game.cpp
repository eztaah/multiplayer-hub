#include "Game.hpp"

#include <iostream>
#include <enet/enet.h>
#include "config.hpp"
#include "NetworkInterface.hpp"
#include <string>
#include <sstream>
#include <array>


enum receiveFromServer
{
    RECEIVE_MY_ID = 1,  // 1 | id
    
    RECEIVE_IDs_ALL_CONNECTED_PLAYERS = 2,  // 2 | id | id | id | ...

    RECEIVE_NEW_PLAYER_CONNECTED = 3,   // 3 | id

    RECEIVE_NEW_POSITION_OTHER_PLAYER = 4,   // 4 | id | posX | posY

    RECEIVE_PLAYER_DISCONNECTED = 5    // 5 | id
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





//////////////////// PUBLIC /////////////////////
Game::Game(sf::RenderWindow& window)
    : _playerManager(),
      _window(window),
      _networkInterface(NetworkInterface::GetInstance()),    // Récupère l'instance de networkInterface
      _myId(-1),
      _lastUpdateTimeEvent1(),
      _clock(),
      _running(false)
{
    // === Initialize ENet and connect to the server ===  
    _networkInterface.Initialize();
    _networkInterface.ConnectToServer("localhost", 7000);
}

void Game::Update() {

    _HandleEvents();


    if(_running)
        _HandleInputs();
}


void Game::Render() {
    // Display all the player
    if(_running) {
        _window.clear(sf::Color::White);
        _playerManager.Render(_window);
    }
    else
        _window.clear(sf::Color::Red);

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

        // Update player newPosition
        _playerManager.ChangePlayerPosition(_myId, newPosition);

        // Send the newPosition to the server
        if(EventTriggered(sf::seconds(0.5f), _lastUpdateTimeEvent1, _clock))
            _networkInterface.SendNewPositionToServer(_myId, newPosition);
    }
}

void Game::_HandleEvents()
{
    // Get all the events
    std::vector<ENetEvent> eventsArray{_networkInterface.GetAllEvents()};

    for(ENetEvent& event : eventsArray)    // event est un raccourci vers une adresse (qui amène vers un evenement)
    {

        switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT:
                printf("> Connection to server succeeded.\n");
                break;

            case ENET_EVENT_TYPE_RECEIVE:
                _HandlePackets(event.packet);
                break;

            case ENET_EVENT_TYPE_DISCONNECT:
                printf("> Disconnected from server.\n");
                // _peer = nullptr;
                break;

            default:
                printf("Evenement inconnu");
                exit(EXIT_FAILURE);
        }
    }
}


void Game::_HandlePackets(ENetPacket* packet)
{
    // Open the packet
    std::vector<std::string> contentArray{_networkInterface.OpenPacket(packet)};

    // Get the tag
    std::string tagStr{contentArray[0]};
    int tag{stoi(tagStr)};
    
    // Gestion des différents cas
    switch (tag)
    {
        case RECEIVE_MY_ID:
        {
            int id{stoi(contentArray[1])};
            // Add my player to the array
            _playerManager.AddPlayer(id);
            // Set my id
            _myId = id;
            std::cout << "L'id qui m'es attribué est le " << id << std::endl;
            // Run the game
            _running = true;
            break;
        }

        case RECEIVE_IDs_ALL_CONNECTED_PLAYERS:
        {
            // Create an array without the tag
            std::vector<std::string> idsArray{contentArray};
            idsArray.erase(idsArray.begin());
            // Send the array
            _playerManager.ManagerAllPlayerIdsReceived(idsArray);
            break;
        }

        case RECEIVE_NEW_PLAYER_CONNECTED:
        {
            int playerId{stoi(contentArray[1])};
            // Add the player to the player array
            _playerManager.AddPlayer(playerId);
            printf("Player %i vient de se connecter.", playerId);
            break;
        }

        case RECEIVE_NEW_POSITION_OTHER_PLAYER:
        {
            // Get the player id 
            int playerId{stoi(contentArray[1])};

            // Get the new player position
            sf::Vector2f newPosition;
            newPosition.x = stoi(contentArray[2]);
            newPosition.y = stoi(contentArray[3]);

            // Met à jour la nouvelle position du joueur dans le tableau
            _playerManager.ChangePlayerPosition(playerId, newPosition);
            printf("Le player %i est maintenant à la position {%f, %f}\n", playerId, newPosition.x, newPosition.y);
            break;
        }

        case RECEIVE_PLAYER_DISCONNECTED:
        {
            // Get the player id 
            int playerId{stoi(contentArray[1])};

            // Remove the player from the array
            _playerManager.RemovePlayer(playerId);
            break;
        }
        
        default:
            printf("Evement inconnu...");
            exit(EXIT_FAILURE);
    }
}