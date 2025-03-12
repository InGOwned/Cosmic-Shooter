#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({640u, 480u}), "SFML");
    window.setFramerateLimit(10);

    sf::CircleShape shape(50.f);
    shape.setOrigin(sf::Vector2f(50.f, 50.f));

    float posX = 0.0, posY = 0.0; // [пикс]
    float velX = 10, velY = 10; // [пикс/сек]

    shape.setPosition(sf::Vector2f(posX, posY));

    sf::Clock clock;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()){
                window.close();
            }
        }

        float dt = clock.getElapsedTime().asSeconds();
        clock.restart();

        posX += velX * dt;
        posY += velY * dt;

        shape.move(sf::Vector2f(velX * dt, velY * dt));

        window.clear();
        window.draw(shape);
        window.display();
    }
}