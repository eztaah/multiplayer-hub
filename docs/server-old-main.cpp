#include "enet/enet.h"

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>

#include <sstream>
#include <vector>
#include <string>

#include <map>



////////////////// CLIENT DATA /////////////
struct Client
{
    ENetPeer *peer;
    int id;
    sf::Vector2f position;
};

int newPlayerId = 0;
std::vector<Client> clientsArray;
////////////////////////////////////////////



////////////////// VARIABLES //////////////
enum sendToClients
{
    SEND_ID_TO_NEW_PLAYER = 1,  // 1 | id
    SEND_IDs_ALL_CONNECTED_PLAYERS  = 2,
    SEND_NEW_PLAYER_CONNECTED = 3,   // 2 | id_color | id_color | ...

    SEND_NEW_POSITION_PLAYER_TO_EVERYONE = 4,   // 3 | id | posX | posY
};


enum receiveFromClient
{
    CLIENT_ASK_FOR_ID = 1,
    CLIENT_ASK_PLAYERS_ALREADY_CONNECTED = 2,
    RECEIVE_NEW_CLIENT_POSITION = 3,
};
////////////////////////////////////////////


////////////////// FUNCTIONS //////////////
std::vector<std::string> splitString(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

void SendPacket(ENetPeer* who, const char* data)
{
    ENetPacket* packet = enet_packet_create(data, strlen(data) + 1, ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(who, 0, packet);
}

void SendPacketToAllExcept(ENetHost *server,  ENetPeer *excludePeer, const char* data) {
    for (size_t i = 0; i < server->peerCount; ++i) {

        ENetPeer *currentPeer = &server->peers[i];

        if (currentPeer->state != ENET_PEER_STATE_CONNECTED) {
            continue; // Skip if the peer is not connected
        }

        if (currentPeer == excludePeer) {
            continue; // Skip the peer who sent the original packet
        }

        SendPacket(currentPeer, data);
    }
}




////////////////// MAIN //////////////
int main() {
    ENetAddress address;
    ENetHost *server;

    // Initialize ENet
    printf("Initializing ENet...\n");
    if (enet_initialize() != 0) {
        fprintf(stderr, "An error occurred while initializing ENet.\n");
        return EXIT_FAILURE;
    }

    // Create a host using ENet
    printf("Creating ENet host...\n");
    address.host = ENET_HOST_ANY;
    address.port = 7000;
    server = enet_host_create(&address, 32, 2, 0, 0);

    if (server == NULL) {
        fprintf(stderr, "An error occurred while creating the ENet server host.\n");
        return EXIT_FAILURE;
    }

    // Main loop for handling events
    printf("Entering main event loop...\n");
    ENetEvent event;
    while (true) {
        
        while (enet_host_service(server, &event, 1000) > 0) {
            //printf("Event occurred: %d\n", event.type);

            switch (event.type) 
            {
                ///////////////////////////////////// CONNECTION ///////////////////////////////////////
                case ENET_EVENT_TYPE_CONNECT:
                {
                    printf("A new client connected from %x:%u.\n", 
                            event.peer->address.host,
                            event.peer->address.port);

                    // incrémente valeur player id + ajoute cet id au tableau
                    newPlayerId++;
                    clientsArray.push_back({event.peer, newPlayerId, {0.0f, 0.0f}});
                    break;
                }

                ///////////////////////////////////// PACKETS ///////////////////////////////////////
                case ENET_EVENT_TYPE_RECEIVE: 
                {
                    
                    // Récupère le paquet
                    unsigned char* x = event.packet->data;      // Récupère le contenu de data
                    size_t length = event.packet->dataLength;    // Récupère la taille de data

                    /**
                     * appel du constructeur qui appartient à la librarie <string>
                     * 
                     * Permet de convertir n'importe quel pointeur ou type intégral en n'importe quel autre pointeur ou type intégral
                     * Contrairement à static_cast, qui effectue des conversions de types qui sont sensées être "sûres" ou "naturelles",
                     * reinterpret_cast effectue une conversion brute des bits, sans vérification ni modification du contenu.
                    */
                    std::string packetData = std::string(reinterpret_cast<char*>(x), length);

                    // Séparation de la chaine de caractère
                    std::vector<std::string> SplittedData{splitString(packetData, '|')};

                    // Get the event type
                    std::string eventTypeStr{SplittedData[0]};
                    int eventType{stoi(eventTypeStr)};

                    // Gère les différents cas
                    switch (eventType)
                    {
                        case CLIENT_ASK_FOR_ID:
                        {
                            // Créé et envoi l'id au joueur
                            int flag{SEND_ID_TO_NEW_PLAYER};
                            std::string data_to_send = std::to_string(flag) + "|" + std::to_string(newPlayerId);
                            printf("Id envoyé au joueur : \" %s \" \n", data_to_send);
                            SendPacket(event.peer, data_to_send.c_str());

                            // Dit à tout les clients que un joueur s'est connecté
                            flag = SEND_NEW_PLAYER_CONNECTED;
                            data_to_send = std::to_string(flag) + "|" + std::to_string(newPlayerId);
                            printf("Id de tt les clients connectés envoyé au joueur : \" %s \" \n", data_to_send);
                            SendPacketToAllExcept(server, event.peer, data_to_send.c_str());
                            break;
                        }

                        case CLIENT_ASK_PLAYERS_ALREADY_CONNECTED:
                        {
                            // Créé le flag
                            int flag{SEND_IDs_ALL_CONNECTED_PLAYERS};

                            // Créé la liste des id
                            std::string result;
                            for(Client& client : clientsArray)
                                // NE PAS ENVOYER MON PROPRE ID
                                result += "|" + std::to_string(client.id);

                            std::string data_to_send = std::to_string(flag) + result;
                            SendPacket(event.peer, data_to_send.c_str());  

                            break;
                        }

                        // UPDATE POSITION
                        case RECEIVE_NEW_CLIENT_POSITION:
                        {
                            // Get the player id
                            int targetID{stoi(SplittedData[1])};

                            // Get the position
                            sf::Vector2f newClientPosition;
                            newClientPosition.x = stoi(SplittedData[2]);
                            newClientPosition.y = stoi(SplittedData[3]);

                            // Modify the position of the player in the array
                            for(Client& client : clientsArray)
                            {
                                if(client.id == targetID)
                                {
                                    client.position = newClientPosition;
                                    std::cout << "Client with ID " << targetID << " updated to position (" << newClientPosition.x << ", " << newClientPosition.y << ").\n";
                                }
                            }

                            // Envoie à tout le monde la nouvelle position du joueur
                            int flag = SEND_NEW_POSITION_PLAYER_TO_EVERYONE;

                            std::string data_to_send = std::to_string(flag) + "|" + std::to_string(targetID) + "|" + std::to_string(newClientPosition.x) + "|" + std::to_string(newClientPosition.y);

                            SendPacketToAllExcept(server, event.peer, data_to_send.c_str());
                            break;
                        }

                        default:
                            break;
                    }
                    
                    enet_packet_destroy(event.packet);
                    break;
                }

                ///////////////////////////////////// DISCONNECTION ///////////////////////////////////////
                case ENET_EVENT_TYPE_DISCONNECT:
                    printf("A client disconnected.\n");
                    break;
            }
        }
    }

    enet_host_destroy(server);
    enet_deinitialize();

    return 0;
}
