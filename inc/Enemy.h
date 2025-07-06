#ifndef ENEMY_H
#define ENEMY_H

#include "GameObjects.h"
#include "Constants.h"

class Enemy : public GameObject {
public:
    Enemy(float x, float y);
    void update();
    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;
    bool isOutOfScreen() const;
};

#endif // ENEMY_H
