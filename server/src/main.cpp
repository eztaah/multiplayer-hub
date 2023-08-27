#include "enet/enet.h"
#include <iostream>


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
            printf("Event occurred: %d\n", event.type);
            switch (event.type) {
                case ENET_EVENT_TYPE_CONNECT:
                    printf("A new client connected.\n");
                    break;

                case ENET_EVENT_TYPE_RECEIVE:
                    printf("Spacebar pressed\n");
                    enet_packet_destroy(event.packet);
                    break;

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
