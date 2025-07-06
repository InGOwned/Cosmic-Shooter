#ifndef BULLET_H
#define BULLET_H

#include "GameObjects.h"
#include "Constants.h"

class Bullet : public GameObject {
public:
    Bullet(float x, float y);
    void update();
    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;
    bool isOutOfScreen() const;
};

#endif // BULLET_H