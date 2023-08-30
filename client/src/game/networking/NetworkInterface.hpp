#pragma once
#include "enet/enet.h"
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

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
    bool ConnectToServer(const char* host, unsigned short port);

    // === Send packets ===
    void SendNewPositionToServer(int myId, sf::Vector2f newPosition);
    void AskIdToServer();
    void AskIdsAllPlayerConnected();

    // === Receving events ===
    void UpdateReceivedEvents();
    std::vector<std::string> GetAllReceivedPackets();


private:
    std::vector<ENetPacket*> _packetsArray;

    ENetHost *_client;
    ENetPeer *_peer;

    NetworkInterface();
    ~NetworkInterface();
};