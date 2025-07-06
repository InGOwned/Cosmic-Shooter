#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H

#include <SFML/Graphics.hpp>

struct GameObject {
    sf::RectangleShape shape;
    sf::Vector2f velocity;
    bool isActive = true;
};

#endif // GAMEOBJECTS_H