#include "Game.hpp"
#include "config.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>


int main() {

    // Create a window with dimensions 800x600 pixels
    sf::RenderWindow window = sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Multiplayer Hub");
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
            switch (event.type)
            {
                // If the window close
                case sf::Event::Closed:
                    window.close();
                    break;

                // Si la fenetres est en focus
                case sf::Event::GainedFocus:
                    WINDOW_IS_FOCUSED = true;
                    break;

                // Si la fenetre n'est plus en focus
                case sf::Event::LostFocus:
                    WINDOW_IS_FOCUSED = false;
                    break;

                default:
                    break;
            }
        }
    }

    return 0;
}
