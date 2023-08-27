#include "NetworkClient.hpp"
#include <iostream>

// Implémentation des méthodes de la classe NetworkClient

NetworkClient::NetworkClient() {
    // Initialize ENet
    if (enet_initialize() != 0) {
        fprintf(stderr, "An error occurred while initializing ENet.\n");
        exit(EXIT_FAILURE);
    }

    client = enet_host_create(NULL, 1, 2, 0, 0);

    if (client == NULL) {
        fprintf(stderr, "An error occurred.\n");
        exit(EXIT_FAILURE);
    }
}

NetworkClient::~NetworkClient() {
    enet_host_destroy(client);
    enet_deinitialize();
}

bool NetworkClient::connectToServer(const char* host, unsigned short port) {
    ENetAddress address;
    enet_address_set_host(&address, host);
    address.port = port;
    peer = enet_host_connect(client, &address, 2, 0);
    return (peer != nullptr);
}

void NetworkClient::sendPacket(const char* message) {
    ENetPacket* packet = enet_packet_create(message, strlen(message) + 1, ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, 0, packet);
}

void NetworkClient::handleEvents() {
    while (enet_host_service(client, &event, 0) > 0) {
        switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT:
                std::cout << "Connection to server succeeded.\n";
                break;

            case ENET_EVENT_TYPE_RECEIVE:
                std::cout << "Packet received.\n";
                enet_packet_destroy(event.packet);
                break;

            case ENET_EVENT_TYPE_DISCONNECT:
                std::cout << "Disconnected from server.\n";
                peer = nullptr;
                break;

            default:
                break;
        }
    }
}
