#include "Game.hpp"
#include "config.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>


int main()
{
    // Create a window with dimensions 800x600 pixels
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML Window Test");
    window.setFramerateLimit(60);

    Game game = Game(window);

    // Main loop that continues until the window is closed
    while (window.isOpen())
    {
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
