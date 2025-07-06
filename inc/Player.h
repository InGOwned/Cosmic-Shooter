#ifndef PLAYER_H
#define PLAYER_H

#include "GameObjects.h"
#include "Constants.h"

class Player : public GameObject {
public:
    Player();
    void update();
    void draw(sf::RenderWindow& window);
    void handleInput();
    sf::FloatRect getBounds() const;
};

#endif // PLAYER_H