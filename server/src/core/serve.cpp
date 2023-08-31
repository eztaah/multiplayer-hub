#include "serve.hpp"
#include <iostream>



//////////////////// UTILS //////////////////////
enum receiveFromClient
{
    RECEIVE_NEW_CLIENT_POSITION = 1,    // 1 | id | posX | posY
};
////////////////////////////////////////////


//////////////////// PUBLIC /////////////////////
Server::Server()
    : _networkInterface(NetworkInterface::GetInstance()),    // Récupère l'instance de networkInterface
      _newClientId(0),
      _clientsArray()
{
    // === Initialize ENet and create the server ===  
    _networkInterface.Initialize();
}


void Server::Update()
{
    // Get all the events
    std::vector<ENetEvent> eventsArray{_networkInterface.GetAllEvents()};

    for(ENetEvent& event : eventsArray)    // event est un raccourci vers une adresse (qui amène vers un evenement)
    {

        switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT:
            {
                printf("A new client connected from %x:%u.\n", event.peer->address.host, event.peer->address.port);

                // Incrémente valeur player id + ajoute cet id au tableau
                _newClientId++;
                _AddClient(event.peer, _newClientId);

                // Envoie l'id et liste des joueurs connecté au joueur
                _networkInterface.SendIdToClient(event.peer, _newClientId);
                _networkInterface.SendIdsOfAllConnectedPlayers(event.peer, _clientsArray);

                // Informe tt le monde qu'un nouveau joueur vient de se connecter
                _networkInterface.SendNewPlayerConnectedToEveryone(event.peer, _newClientId);

                break;
            }

            case ENET_EVENT_TYPE_RECEIVE:
                _HandlePacket(event);
                break;

            case ENET_EVENT_TYPE_DISCONNECT:
            {
                // Remove client from array
                int id = _RemoveClient(event.peer);

                _networkInterface.SendIdPlayerDisconnectedToEveryone(event.peer, id);

                break;
            }


            default:
                printf("Evenement inconnu");
                exit(EXIT_FAILURE);
        }
    }
}











///////////////////////////////////// PRIVATE ////////////////////////////////////
void Server::_AddClient(ENetPeer* peer, const int id) {
    _clientsArray.push_back(Client(peer, id, {0.0f, 0.0f}));
}

int Server::_RemoveClient(ENetPeer* peer) {
    int index;
    int id;

    for(unsigned int i{0} ; i < _clientsArray.size() ; ++i) {
        if(_clientsArray[i].peer == peer) {
            index = i;
            id = _clientsArray[i].id;
        }

    }
    _clientsArray.erase(_clientsArray.begin() + index);

    return id;
}


void Server::_HandlePacket(ENetEvent event)
{

    // Open the packet
    ENetPacket* packet{event.packet};
    std::vector<std::string> contentArray{_networkInterface.OpenPacket(packet)};

    // Get the tag
    std::string tagStr{contentArray[0]};
    int tag{stoi(tagStr)};
    
    // Gestion des différents cas
    switch (tag)
    {
        case RECEIVE_NEW_CLIENT_POSITION:
        {
            // === Update the position ===

            // Get the player id
            int playerId{stoi(contentArray[1])};

            // Get the position
            sf::Vector2f newClientPosition;
            newClientPosition.x = stoi(contentArray[2]);
            newClientPosition.y = stoi(contentArray[3]);

            // Modify the position of the player in the array
            for(Client& client : _clientsArray)
            {
                if(client.id == playerId)
                {
                    client.position = newClientPosition;
                    std::cout << "Client with ID " << playerId << " updated to position (" << newClientPosition.x << ", " << newClientPosition.y << ").\n";
                }
            }

            // === Send to everyone the new position ===
            _networkInterface.SendNewPlayerPositionToEveryone(event.peer, playerId, newClientPosition);
            break;
        }


        default:
            printf("Evement inconnu...");
            exit(EXIT_FAILURE);
    }
}