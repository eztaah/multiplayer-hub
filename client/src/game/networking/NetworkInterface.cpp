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
    : _client(),
      _peer()
{}

NetworkInterface::~NetworkInterface() {}




//////////////////// UTILS //////////////////////
enum sendToServer
{
    SEND_NEW_POS = 1,    // 1 | id | posX | posY
};


void SendPacketToServer(ENetPeer* peer, int tag, std::string content)
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
        printf("A packet containing : \"%s\" has been sent to the server\n", data);
    }
}

std::string IntToFourDigitString(int number) {
    std::ostringstream os;
    os << std::setw(4) << std::setfill('0') << number;
    return os.str();
}

std::vector<std::string> SplitString(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
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

    // === Créé un client ===
    /**
     * adress : NULL -> Créé un client qui n'est pas lié à une adresse IP particulière
     * peerCount : 1 -> Le client peut se connecter à un seul serveur à la fois
     * channelLimit : 2 -> Le client peut utiliser 2 canaux différents pour envoyer des messages
     * incomingBandwidth : 0 -> 
     * outgoingBandwidth : 0 ->
    */
    _client = enet_host_create(NULL, 1, 2, 0, 0);
    if (_client != NULL) {
        printf("The client has been successfully created.\n");
    } else {
        printf("An error occurred while creating the client.\n");
        exit(EXIT_FAILURE);
    }
}

void NetworkInterface::Deinitialize()
{
    enet_host_destroy(_client);
    enet_deinitialize();
}

bool NetworkInterface::ConnectToServer(const char* host, unsigned short port)
{
    // === Mise en place de l'adresse du serveur ===
    ENetAddress address;    // Struct qui contient host et port
    enet_address_set_host(&address, host);  // Rempli le champs host avec l'adresse IP du serveur
    address.port = port;    // On défini manuellement le port

    // === Connection au serveur ===
    /**
     * peer est un pointeur vers une structure 'ENetPeer'. Cette structure repésente une connection à un autre hote (ici le serveur)
     * peer désigne donc une connection entre le client et un hote (ici serveur)
    */
    _peer = enet_host_connect(_client, &address, 2, 0);      // 2 -> Nombres de canaux de cette connection

    return (_peer != nullptr);
}


// === Send packets ===
void NetworkInterface::SendNewPositionToServer(int myId, sf::Vector2f newPosition)
{
    // Create the data
    std::string myIdStr{std::to_string(myId)};
    std::string posX{IntToFourDigitString(newPosition.x)}; 
    std::string posY{IntToFourDigitString(newPosition.y)}; 
    std::string dataStr{myIdStr + "|" + posX + "|" + posY};

    SendPacketToServer(_peer, SEND_NEW_POS, dataStr);
}


// === Events management ===
std::vector<ENetEvent> NetworkInterface::GetAllEvents()
{
    ENetEvent event;

    // === Vide le tableau d'events ===
    _eventsArray.clear();

    // === Add all the events to the array ===
    while (enet_host_service(_client, &event, 0) > 0)
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




