#pragma once
#include <vector>
#include <SFML/System.hpp>
#include "NetworkInterface.hpp"
#include <enet/enet.h>
#include "global.hpp"


class Server
{
public:
    Server();

    void Update();

private:
    void _AddClient(ENetPeer* peer, const int id);
    int _RemoveClient(ENetPeer* peer);
    void _HandlePacket(ENetEvent event);

    NetworkInterface& _networkInterface;
    int _newClientId;
    std::vector<Client> _clientsArray;
};