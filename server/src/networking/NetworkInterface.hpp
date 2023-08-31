#pragma once
#include "enet/enet.h"
#include "global.hpp"
#include <vector>
#include <string>
#include <SFML/System.hpp>


////////// SINGLETON /////
class NetworkInterface
{
public:
    // Méthode pour accéder à l'instance du Singleton
    static NetworkInterface& GetInstance();

    // Supprimer les constructeurs et opérateurs de copie/mouvement
    NetworkInterface(NetworkInterface const&) = delete;
    void operator=(NetworkInterface const&)  = delete;

    void Initialize();
    void Deinitialize();

    // === Send packets ===
    void SendIdToClient(ENetPeer* peer, int clientId);
    void SendIdsOfAllConnectedPlayers(ENetPeer* peer, const std::vector<Client>& clientsArray);
    void SendNewPlayerConnectedToEveryone(ENetPeer* peer, int clientId);
    void SendNewPlayerPositionToEveryone(ENetPeer* peer, int playerId, const sf::Vector2f newClientPosition);
    void SendIdPlayerDisconnectedToEveryone(ENetPeer* peer, int clientId);


    // === Receving events ===
    std::vector<ENetEvent> GetAllEvents();
    std::vector<std::string> OpenPacket(ENetPacket*& packet);


private:

    ENetHost *_server;
    std::vector<ENetEvent> _eventsArray;
    
    NetworkInterface();
    ~NetworkInterface();
};