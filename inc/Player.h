#ifndef PLAYER_H
#define PLAYER_H

#pragma once
#include "GameObjects.h"
#include "Constants.h"
#include <SFML/Graphics.hpp>

class Player : public GameObject {
public:
    Player();
    void update();
    void draw(sf::RenderWindow& window);
    void handleInput();
    void resetPosition();

    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const;
    sf::Vector2f getSize() const;

private:
    sf::Texture texture;
    sf::Sprite sprite;
};

#endif // PLAYER_H