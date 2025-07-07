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
    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const; // Новый метод
    sf::Vector2f getSize() const;

private:
    sf::Texture texture;
    sf::Sprite sprite;
    // sf::Vector2f velocity; // Удалено, так как уже есть в GameObject
};

#endif // PLAYER_H