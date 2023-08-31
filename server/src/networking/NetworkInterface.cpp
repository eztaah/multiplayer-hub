#include "NetworkInterface.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>



// Méthode pour accéder à l'instance du Singleton
NetworkInterface& NetworkInterface::GetInstance() {
    static NetworkInterface instance; // Instantié au premier appel
    return instance;
}


NetworkInterface::NetworkInterface() 
    : _server(),
      _eventsArray()
{}

NetworkInterface::~NetworkInterface() {}




//////////////////// UTILS //////////////////////
enum sendToClient
{
    SEND_ID_TO_NEW_PLAYER = 1,  // 1 | id

    SEND_IDs_ALL_CONNECTED_PLAYERS  = 2,    // 2 | id | id | id | ...

    SEND_NEW_PLAYER_CONNECTED = 3,   // 3 | id

    SEND_NEW_POSITION_PLAYER_TO_EVERYONE = 4,   // 4 | id | posX | posY

    SEND_PLAYER_DISCONNECTED = 5    // 5 | id
};


std::vector<std::string> SplitString(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

void SendPacketToClient(ENetPeer* peer, int tag, std::string content)
{
    // === Format the data ===
    std::string dataStr{std::to_string(tag) + "|" + content};
    const char* data{dataStr.c_str()};

    // === Create the packet ===
    ENetPacket* packet = enet_packet_create(data, strlen(data) + 1, ENET_PACKET_FLAG_RELIABLE);

    // === Send the packet ===
    if(peer == nullptr)
        std::cout << "Cannot send the packet, peer is a nullptr.\n" << std::endl;
    else {
        enet_peer_send(peer, 0, packet);
        printf("A packet containing : \"%s\" has been sent to the client\n", data);
    }
}

void SendPacketToAllExcept(ENetHost *server,  ENetPeer *excludePeer, int tag, std::string content) {
    for (size_t i = 0; i < server->peerCount; ++i) {

        ENetPeer *currentPeer = &server->peers[i];

        if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
            continue; // Skip if the peer is not connected
        if (currentPeer == excludePeer) 
            continue; // Skip the peer who sent the original packet

        SendPacketToClient(currentPeer, tag, content);
    }
}


////////////////////////////////////////////////



void NetworkInterface::Initialize()
{
    // === Initialize ENet ===
    if (enet_initialize() == 0) {
        printf("Enet has been successfully initialised. \n");
    } else {
        printf("An error occurred while initializing ENet.\n");
        exit(EXIT_FAILURE);
    }

    // Create a host using ENet
    ENetAddress address;
    address.host = ENET_HOST_ANY;
    address.port = 7000;
    _server = enet_host_create(&address, 32, 2, 0, 0);

    if (_server != NULL) {
        printf("The server has been successfully created.\n");
    }
    else {
        printf("An error occurred while creating the ENet server host.\n");
        exit(EXIT_FAILURE);
    }
}


void NetworkInterface::Deinitialize()
{
    enet_host_destroy(_server);
    enet_deinitialize();
}


// === Send packets ===
void NetworkInterface::SendIdToClient(ENetPeer* peer, int clientId)
{
    // Create the data
    std::string dataStr{std::to_string(clientId)};
    
    // Send the data
    SendPacketToClient(peer, SEND_ID_TO_NEW_PLAYER, dataStr);
}


void NetworkInterface::SendIdsOfAllConnectedPlayers(ENetPeer* peer, const std::vector<Client>& clientsArray)
{
    // Création d'un array qui contient tt les autres joueurs connectés
    std::string dataStr;
    bool succeed{false};
    for(const Client& client : clientsArray) {
        if(peer != client.peer) {
            dataStr += std::to_string(client.id) + "|";
            succeed = true;
        }
    }
    if(succeed)
        dataStr.pop_back();
    else
        dataStr = "-1";

    // Send the data
    SendPacketToClient(peer, SEND_IDs_ALL_CONNECTED_PLAYERS, dataStr);
}

void NetworkInterface::SendNewPlayerConnectedToEveryone(ENetPeer* peer, int clientId)
{
    std::string dataStr{std::to_string(clientId)};

    SendPacketToAllExcept(_server, peer, SEND_NEW_PLAYER_CONNECTED, dataStr);
}

void NetworkInterface::SendNewPlayerPositionToEveryone(ENetPeer* peer, int playerId, const sf::Vector2f newClientPosition)
{
    std::string dataStr{std::to_string(playerId) + "|" + std::to_string(newClientPosition.x) + "|" + std::to_string(newClientPosition.y)};

    SendPacketToAllExcept(_server, peer, SEND_NEW_POSITION_PLAYER_TO_EVERYONE, dataStr);
};

void NetworkInterface::SendIdPlayerDisconnectedToEveryone(ENetPeer* peer, int clientId)
{
    std::string dataStr{std::to_string(clientId)};

    SendPacketToAllExcept(_server, peer, SEND_PLAYER_DISCONNECTED, dataStr);   
}




// === Receving events ===
std::vector<ENetEvent> NetworkInterface::GetAllEvents()
{
    // === Vide le tableau d'events ===
    _eventsArray.clear();


    ENetEvent event;

    // === Add all the events to the array ===
    while (enet_host_service(_server, &event, 0) > 0)
    {
        _eventsArray.push_back(event); // ajouter au tableau l'adresse de event
    }

    return _eventsArray;
}

std::vector<std::string> NetworkInterface::OpenPacket(ENetPacket*& packet)
{
    // Récupère le paquet
    unsigned char* data = packet->data;      // Récupère le contenu de data
    size_t length = packet->dataLength;    // Récupère la taille de data

    // Convertis data en un std::string et detruit le packet
    std::string dataStr = std::string(reinterpret_cast<char*>(data), length);
    enet_packet_destroy(packet);

    // Convertis la chaine de caratère en un tableau contenant les données
    std::vector<std::string> content{SplitString(dataStr, '|')};

    return content;
}
