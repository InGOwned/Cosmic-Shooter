#ifndef BULLET_H
#define BULLET_H

#pragma once
#include "GameObjects.h"
#include "Constants.h"
#include <SFML/Graphics.hpp>

class Bullet : public GameObject {
public:
    Bullet(float x, float y);
    void update();
    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;
    bool isOutOfScreen() const;

    bool isActive = true;  // Добавлен публичный член

private:
    sf::RectangleShape shape;
    sf::Vector2f velocity;
};

#endif // BULLET_H
