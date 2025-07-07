#ifndef ENEMY_H
#define ENEMY_H

#pragma once
#include "GameObjects.h"
#include "Constants.h"
#include <SFML/Graphics.hpp>

class Enemy : public GameObject {
public:
    Enemy(float x, float y);
    void update();
    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;
    bool isOutOfScreen() const;
    bool isActive = true;  // Добавлен публичный член

private:
    sf::RectangleShape shape;
    sf::Vector2f velocity;
};

#endif // ENEMY_H
