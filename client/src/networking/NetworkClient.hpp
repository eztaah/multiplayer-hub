#pragma once
#include <enet/enet.h>

class NetworkClient {
public:
    NetworkClient();
    ~NetworkClient();
    bool connectToServer(const char* host, unsigned short port);
    void sendPacket(const char* message);
    void handleEvents();
private:
    ENetHost *client;
    ENetPeer *peer;
    ENetEvent event;
};
