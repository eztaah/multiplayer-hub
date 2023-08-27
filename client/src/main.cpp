#include "Game.hpp"
#include "config.hpp"
#include "NetworkClient.hpp"
#include <SFML/Graphics.hpp>
#include <enet/enet.h>
#include <iostream>


int main()
{
    // Create a window with dimensions 800x600 pixels
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML Window Test");
    window.setFramerateLimit(60);

    // Initialize the client
    NetworkClient client = NetworkClient();
    // Connect the client to the server
    if(!client.connectToServer("localhost", 7000))
        return EXIT_FAILURE;

    Game game = Game(window);

    // Main loop that continues until the window is closed
    while (window.isOpen())
    {
        // If the spacebar is pressed, send a packet to the server
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            client.sendPacket("Spacebar pressed");

        // Handle incoming events
        client.handleEvents();

        //// UPDATE ///
        game.Update();
        game.Render();

        // Event processing
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
    }

    return 0;
}
